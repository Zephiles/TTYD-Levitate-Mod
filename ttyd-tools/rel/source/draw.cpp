#include "draw.h"
#include "main.h"

#include <gc/mtx.h>
#include <ttyd/dispdrv.h>
#include <ttyd/windowdrv.h>
#include <ttyd/fontmgr.h>

#include <cstring>
#include <cstdio>

char displayBuffer[256];
const char *versionNumberString = "v1.0";

void drawFunctionOnDebugLayer(void (*func)())
{
    ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 2, 0.f, 
        [](ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        reinterpret_cast<void (*)()>(user)();
    }, reinterpret_cast<void *>(func));
}

void drawFunctionOnDebugLayerWithOrder(void (*func)(), float order)
{
    ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::kDebug3d, 2, order, 
        [](ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        reinterpret_cast<void (*)()>(user)();
    }, reinterpret_cast<void *>(func));
}

void drawFunctionOn2DLayer(void (*func)())
{
    ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::k2d, 2, 0.f, 
        [](ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        reinterpret_cast<void (*)()>(user)();
    }, reinterpret_cast<void *>(func));
}

void drawFunctionOn2DLayerWithOrder(void (*func)(), float order)
{
    ttyd::dispdrv::dispEntry(ttyd::dispdrv::CameraId::k2d, 2, order, 
        [](ttyd::dispdrv::CameraId cameraId, void *user)
    {
        (void)cameraId;
        reinterpret_cast<void (*)()>(user)();
    }, reinterpret_cast<void *>(func));
}

void drawWindow(uint32_t color, int32_t x, int32_t y, int32_t width, int32_t height, int32_t curve)
{
    constexpr int32_t numValues = 5;
    int32_t values[numValues] = { x, y, width, height, curve };
    float valuesOut[numValues];
    
    intToFloatArray(values, valuesOut, numValues);
    uint8_t *newColor = reinterpret_cast<uint8_t *>(&color);
    
    ttyd::windowdrv::windowDispGX_Waku_col(0, newColor, valuesOut[0], 
        valuesOut[1], valuesOut[2], valuesOut[3], valuesOut[4]);
}

void drawText(const char *text, int32_t x, int32_t y, uint32_t color, float scale)
{
    // Make sure the text isn't an empty string
    if (text[0] == '\0')
    {
        return;
    }
    
    gc::mtx::mtx34 mtxScale;
    gc::mtx::PSMTXScale(mtxScale, scale, scale, scale);
    
    gc::mtx::mtx34 mtxTrans;
    gc::mtx::PSMTXTransApply(
        mtxScale, 
        mtxTrans, 
        intToFloat(x), 
        intToFloat(y), 
        0.f);
    
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    ttyd::fontmgr::FontDrawMessageMtx(mtxTrans, text);
}

// Credits to Jdaster64 for writing the original code for this function
void drawTextMultipleLines(const char *text, int32_t x, int32_t y, uint32_t color, float scale)
{
    char lineBuffer[128];
    const char *currentLine = text;
    constexpr int32_t maxLength = sizeof(lineBuffer) - 1;
    
    // Draw each individual line
    while (1)
    {
        // Find the end of the current line
        const char *newline = strchr(currentLine, '\n');
        
        // If a newline is not found, then currently at the last line
        if (!newline)
        {
            break;
        }
        
        // Copy this line to the temporary buffer and append a null byte
        int32_t lineLength = newline - currentLine;
        
        // Make sure the current line won't be an empty string
        if (lineLength > 0)
        {
            // Prevent a buffer overflow
            if (lineLength > maxLength)
            {
                lineLength = maxLength;
            }
            
            char *tempBuffer = strncpy(lineBuffer, currentLine, lineLength);
            tempBuffer[lineLength] = '\0';
            
            drawText(tempBuffer, x, y, color, scale);
        }
        
        // Advance to the next line
        currentLine = newline + 1;
        y -= 20;
    }
    
    // Draw the rest of the text
    drawText(currentLine, x, y, color, scale);
}

void drawTextInit(uint8_t alpha, bool drawFontEdge)
{
    ttyd::fontmgr::FontDrawStart_alpha(alpha);
    
    // Check if the font edge should be on or off
    if (drawFontEdge)
    {
        ttyd::fontmgr::FontDrawEdge();
    }
}

void drawTextAndInit(const char *text, int32_t x, int32_t y, 
    uint8_t alpha, uint32_t color, bool drawFontEdge, float scale)
{
    drawTextInit(alpha, drawFontEdge);
    drawText(text, x, y, color, scale);
}

void drawTextMultipleLinesAndInit(const char *text, int32_t x, int32_t y, 
    uint8_t alpha, uint32_t color, bool drawDontEdge, float scale)
{
    drawTextInit(alpha, drawDontEdge);
    drawTextMultipleLines(text, x, y, color, scale);
}

void drawTitleScreenInfo()
{
    // Draw the window for the text
    uint32_t windowColor = 0x000000E5;
    int32_t posX         = -225;
    int32_t posY         = -16;
    int32_t windowWidth  = 450;
    int32_t windowHeight = 70;
    int32_t curve        = 10;
    
#ifdef TTYD_JP
    posY += 30;
    windowWidth -= 10;
#endif
    
    drawWindow(windowColor, posX, posY, windowWidth, windowHeight, curve);
    
    // Draw the text
    uint32_t textColor = 0xFFFFFFFF;
    uint8_t alpha      = 0xFF;
    float scale        = 0.75f;
    posX               += 119;
    posY               -= 14;
    
    char *tempDisplayBuffer = displayBuffer;
    sprintf(tempDisplayBuffer,
        "Levitate Mod %s\nCreated by Zephiles",
        versionNumberString);
    
    drawTextMultipleLinesAndInit(tempDisplayBuffer, posX, posY, alpha, textColor, false, scale);
}