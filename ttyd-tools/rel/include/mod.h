#pragma once

#include <ttyd/seqdrv.h>

#include <cstdint>

namespace mod {

class Mod
{
public:
    Mod();
    void init();
    
private:
    void runOncePerFrame();
    void displayTitleScreenInfo(ttyd::seqdrv::SeqInfo *);
    
    void (*mPFN_marioStMain_trampoline)() = nullptr;
    void (*mPFN_titleMain_trampoline)(ttyd::seqdrv::SeqInfo *) = nullptr;
};

}