#ifndef _HELPERS_
#define _HELPERS_

#include <Arduino.h>
#include "Globals.h"

void bigFont();
void smallFont();

void clearMatrix();

// Quick and easy printing of text on the matrix
void printOnMatrix(String message, uint16_t color = dma_display->color565(0, 255, 0));

void message(String message, bool onDisplay = false);


#endif