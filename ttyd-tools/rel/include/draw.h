#pragma once

#include <cstdint>

extern char displayBuffer[512];
extern const char *versionNumberString;

void drawFunctionOnDebugLayer(void (*func)());
void drawFunctionOnDebugLayerWithOrder(void (*func)(), float order);
void drawFunctionOn2DLayer(void (*func)());
void drawFunctionOn2DLayerWithOrder(void (*func)(), float order);

void drawWindow(uint32_t color, int32_t x, int32_t y, int32_t width, int32_t height, int32_t curve);

void drawStringMultiline(float x, float y, const char *text);
void drawText(const char *text, int32_t x, int32_t y, uint32_t color, float scale);
void drawTextInit(uint8_t alpha, bool drawFontEdge);

void drawTextAndInit(const char *text, int32_t x, int32_t y, 
    uint8_t alpha, uint32_t color, bool drawDontEdge, float scale);

void drawTitleScreenInfo();