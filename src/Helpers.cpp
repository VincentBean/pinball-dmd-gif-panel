#include <Arduino.h>
#include "Globals.h"
#include "Fonts/FreeSansBoldOblique12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"

void bigFont()
{
  dma_display->setFont(&FreeSansBoldOblique12pt7b);
  dma_display->setCursor(14, 23);
  dma_display->setTextSize(1);
}

void smallFont()
{
  dma_display->setFont(&FreeSans9pt7b);
  dma_display->setCursor(0, 14);
  dma_display->setTextSize(1);
}

void clearMatrix()
{
    dma_display->clearScreen();
    dma_display->fillScreen(dma_display->color565(0, 0, 0));
}

// Quick and easy printing of text on the matrix
void printOnMatrix(String message, uint16_t color = dma_display->color565(0, 255, 0))
{
    clearMatrix();
    smallFont();

    dma_display->setTextColor(color);

    dma_display->println(message);
}

void message(String message, bool onDisplay = false)
{
    Serial.println(message);

    if (onDisplay) {
        printOnMatrix(message);
    }

}
