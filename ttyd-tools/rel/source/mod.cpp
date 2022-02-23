#include "mod.h"
#include "patch.h"
#include "main.h"
#include "draw.h"

#include <ttyd/mariost.h>
#include <ttyd/seq_title.h>

namespace mod {

Mod *gMod = nullptr;

void main()
{
    Mod *mod = new Mod();
    mod->init();
}

Mod::Mod()
{
    
}

void Mod::init()
{
    gMod = this;
    performBootPatches();
    
    mPFN_marioStMain_trampoline = patch::hookFunction(
        ttyd::mariost::marioStMain, []()
    {
        gMod->runOncePerFrame();
    });
    
    mPFN_titleMain_trampoline = patch::hookFunction(
        ttyd::seq_title::titleMain, [](ttyd::seqdrv::SeqInfo *seqInfo)
    {
        gMod->displayTitleScreenInfo(seqInfo);
    });
}

void Mod::runOncePerFrame()
{
    levitate();
    
    // Call the original function
    mPFN_marioStMain_trampoline();
}

void Mod::displayTitleScreenInfo(ttyd::seqdrv::SeqInfo *seqInfo)
{
    // Check to see if the title screen is ready
    // Check to see if the curtain is fully displayed or not
    uint32_t titleMainCheck = *reinterpret_cast<uint32_t *>(
        reinterpret_cast<uint32_t>(ttyd::seq_title::seqTitleWorkPointer2) + 0x8);
    
    if ((titleMainCheck >= 2) && (titleMainCheck < 12))
    {
        // Curtain is not fully displayed
        // Draw the title screen info
        drawFunctionOn2DLayerWithOrder(drawTitleScreenInfo, -1.f);
    }
    
    // Call the original function
    mPFN_titleMain_trampoline(seqInfo);
}

}