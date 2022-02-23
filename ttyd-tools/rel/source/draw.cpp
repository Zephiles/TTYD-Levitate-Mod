#include "draw.h"
#include "main.h"

#include <ttyd/dispdrv.h>
#include <ttyd/windowdrv.h>
#include <ttyd/fontmgr.h>

#include <cstring>
#include <cstdio>

char displayBuffer[512];
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
    uint8_t *newColor = reinterpret_cast<uint8_t *>(&color);
    float newX        = static_cast<float>(x);
    float newY        = static_cast<float>(y);
    float newWidth    = static_cast<float>(width);
    float newHeight   = static_cast<float>(height);
    float newCurve    = static_cast<float>(curve);
    
    ttyd::windowdrv::windowDispGX_Waku_col(0, newColor, newX, newY, newWidth, newHeight, newCurve);
}

// Replace the first \n found with \0, and return the index of the character after that
uint32_t getNextLineIndex(char *str)
{
    // Set the initial index
    uint32_t i = 0;
    
    // Loop through the string until \n or \0 is found
    while ((str[i] != '\n') && (str[i] != '\0'))
    {
        i++;
    }
    
    // Replace \n with \0 and increment the index to the next line
    if (str[i] != '\0')
    {
        str[i] = '\0';
        i++;
    }
    
    // Return the index of the next line
    // Returns 0 when at the end of the string
    return i;
}

void drawStringMultiline(float x, float y, const char *text)
{
    // Copy the text to a temporary array, as it will be modified
    uint32_t textSize = strlen(text);
    char tempText[textSize + 1];
    strcpy(tempText, text);
    
    // Get the index for the next line
    uint32_t index = getNextLineIndex(tempText);
    
    // Draw the first line
    char *currentLine = tempText;
    
    do
    {
        // Only draw the line if not pointing to an empty string
        // This will only occur if multiple newlines were directly next to each other
        if (currentLine[0] != '\0')
        {
            ttyd::fontmgr::FontDrawString(x, y, currentLine);
        }
        
        // Set currentLine to the next line
        currentLine += index;
        
        // Get the following line index
        index = getNextLineIndex(currentLine);
        
        // Implement the new line space
        y -= 20.f;
    }
    while (index != 0);
}

void drawText(const char *text, int32_t x, int32_t y, uint32_t color, float scale)
{
    ttyd::fontmgr::FontDrawScale(scale);
    ttyd::fontmgr::FontDrawColor(reinterpret_cast<uint8_t *>(&color));
    
    uint32_t i = 0;
    char endOfLineChar;
    bool loopDone = false;
    
    while (!loopDone)
    {
        endOfLineChar = text[i];
        if ((endOfLineChar == '\n') || (endOfLineChar == '\0'))
        {
            loopDone = true;
        }
        i++;
    }
    
    float newX = static_cast<float>(x);
    float newY = static_cast<float>(y);
    
    if (endOfLineChar == '\n')
    {
        // The text has multiple lines
        drawStringMultiline(newX, newY, text);
    }
    else
    {
        // The text has one line
        ttyd::fontmgr::FontDrawString(newX, newY, text);
    }
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

void drawTextAndInit(const char *text, int32_t x, int32_t y, uint8_t alpha, uint32_t color, bool drawFontEdge, float scale)
{
    drawTextInit(alpha, drawFontEdge);
    drawText(text, x, y, color, scale);
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
    
    drawTextAndInit(tempDisplayBuffer, posX, posY, alpha, textColor, false, scale);
}