#pragma once

#include <cstdint>

extern "C" {

// Assembly overwrite functions
#ifdef TTYD_JP
void asmBacktraceScreenIncrementYPos();
#endif

// Functions accessed by assembly overwrites
#ifdef TTYD_JP
int32_t backtraceScreenIncrementYPos();
#endif

}