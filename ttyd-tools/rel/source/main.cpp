#include "main.h"
#include "patch.h"
#include "assembly.h"

#include <ttyd/seq_title.h>
#include <ttyd/system.h>
#include <ttyd/seqdrv.h>
#include <ttyd/mario.h>

int32_t backtraceScreenIncrementYPos()
{
    constexpr int32_t kPosYIncrementAmount = 50;
    static int16_t incrementCount = 0;
    int32_t tempIncrementCount = incrementCount;
    
    int32_t textPosY = tempIncrementCount + kPosYIncrementAmount;
    tempIncrementCount--;
    
    if (tempIncrementCount < -960)
    {
        // Simulate incrementing exactly once to match the US/PAL code
        incrementCount = -1;
        return kPosYIncrementAmount;
    }
    else
    {
        incrementCount = static_cast<int16_t>(tempIncrementCount);
        return textPosY;
    }
}

void performBootPatches()
{
#ifdef TTYD_US
    void *debugModeInitialzeAddress           = reinterpret_cast<void *>(0x80009B2C);
    void *debugModeShowBuildDateAddress       = reinterpret_cast<void *>(0x80008FE8);
    void *backtraceScreenFontSizeAddress      = reinterpret_cast<void *>(0x80428BC0);
    void *backtraceScreenPPCHaltBranchAddress = reinterpret_cast<void *>(0x8025E4A4);
    void *backtraceScreenEndBranchAddress     = reinterpret_cast<void *>(0x8025E4A8);
#elif defined TTYD_JP
    void *debugModeInitialzeAddress           = reinterpret_cast<void *>(0x8000999C);
    void *debugModeShowBuildDateAddress       = reinterpret_cast<void *>(0x80008EB8);
    void *backtraceScreenPosYValueAddress     = reinterpret_cast<void *>(0x802582F8);
    void *backtraceScreenFontSizeAddress      = reinterpret_cast<void *>(0x80422618);
#elif defined TTYD_EU
    void *debugModeInitialzeAddress           = reinterpret_cast<void *>(0x80009CF0);
    void *debugModeShowBuildDateAddress       = reinterpret_cast<void *>(0x800091B4);
    void *backtraceScreenFontSizeAddress      = reinterpret_cast<void *>(0x804356C8);
    void *backtraceScreenPPCHaltBranchAddress = reinterpret_cast<void *>(0x8026207C);
    void *backtraceScreenEndBranchAddress     = reinterpret_cast<void *>(0x80262080);
#endif
    
    // Enable debug mode by default
    *reinterpret_cast<uint32_t *>(debugModeInitialzeAddress) = 0x3800FFFF; // li r0,-1
    
    // Set the initial value for the debug mode variable
    *reinterpret_cast<int32_t *>(
        reinterpret_cast<uint32_t>(
            ttyd::seq_title::seqTitleWorkPointer2) + 0x30) = -1;
    
    // Show build date on title screen by default
    *reinterpret_cast<uint32_t *>(debugModeShowBuildDateAddress) = 0x60000000; // nop
    
    // Change the text size on the backtrace screen
    *reinterpret_cast<float *>(backtraceScreenFontSizeAddress) = 0.66f;
    
    // Make the backtrace screen loop
    #ifdef TTYD_JP
        mod::patch::writeBranchBL_Template(backtraceScreenPosYValueAddress, asmBacktraceScreenIncrementYPos);
    #else
        *reinterpret_cast<uint32_t *>(backtraceScreenPPCHaltBranchAddress)  = 0x3B400000; // li r26,0
        *reinterpret_cast<uint32_t *>(backtraceScreenEndBranchAddress)      = 0x4BFFFDD4; // b -0x22C
    #endif
}

float __attribute__ ((noinline)) intToFloat(int32_t value)
{
    return static_cast<float>(value);
}

void intToFloatArray(int32_t *values, float *valuesOut, int32_t numValues)
{
    for (int32_t i = 0; i < numValues; i++)
    {
        valuesOut[i] = intToFloat(values[i]);
    }
}

bool checkForSpecificSeq(ttyd::seqdrv::SeqIndex wantedSeq)
{
    ttyd::seqdrv::SeqIndex nextSeq = ttyd::seqdrv::seqGetNextSeq();
    if (nextSeq != wantedSeq)
    {
        return false;
    }
    
    ttyd::seqdrv::SeqIndex currentSeq = ttyd::seqdrv::seqGetSeq();
    return currentSeq == wantedSeq;
}

bool checkButtonCombo(uint32_t combo)
{
    uint32_t buttonInput = ttyd::system::keyGetButton(0);
    if ((buttonInput & combo) != combo)
    {
        return false;
    }
    
    uint32_t buttonInputTrg = ttyd::system::keyGetButtonTrg(0);
    return buttonInputTrg & combo;
}

bool checkButtonComboEveryFrame(uint32_t combo)
{
    uint32_t buttonInput = ttyd::system::keyGetButton(0);
    return (buttonInput & combo) == combo;
}

void levitate()
{
    constexpr uint32_t levitateButtonCombo = PAD_L | PAD_A;
    if (checkButtonComboEveryFrame(levitateButtonCombo))
    {
        ttyd::mario::Player *player = ttyd::mario::marioGetPtr();
        player->wJumpVelocityY = 4.540000915527344f;
    }
}