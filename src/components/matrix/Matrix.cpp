#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Globals.h"

#define PANEL_RES_X 64 // Number of pixels wide of each INDIVIDUAL panel module.
#define PANEL_RES_Y 32 // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 2  // Total number of panels chained one to another

MatrixPanel_I2S_DMA *dma_display = nullptr;

void InitMatrix()
{
 HUB75_I2S_CFG mxconfig(
      PANEL_RES_X, // module width
      PANEL_RES_Y, // module height
      PANEL_CHAIN  // Chain length
  );

  mxconfig.gpio.e = 18;
  // mxconfig
  mxconfig.clkphase = false;
  mxconfig.latch_blanking = 4;
  mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_10M;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(90); // 0-255
  dma_display->clearScreen();
  
  dma_display->begin();

  dma_display->fillScreen(dma_display->color565(0, 0, 0));

}