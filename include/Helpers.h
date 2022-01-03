#pragma once

#include <Arduino.h>
#include "Globals.h"

void message(String message, bool onDisplay = false)
{
    Serial.println(message);

    if (onDisplay) {
        printOnMatrix(message);
    }

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

    dma_display->setCursor(0, 0);
    dma_display->setTextSize(1);
    dma_display->setTextColor(color);

    dma_display->println(message);
}