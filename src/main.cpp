#include <Arduino.h>
#include <SdFat.h>
#include <AnimatedGIF.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Globals.h"
#include "components/gif/MatrixGif.hpp"
#include "components/gif/GifLoader.hpp"
#include "components/gif/GifPlayer.hpp"
#include "components/wifi/WiFi.hpp"

#define PANEL_RES_X 64      // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_RES_Y 32     // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 2      // Total number of panels chained one to another
 
const uint8_t SD_CS_PIN = 22;
//
// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = 32;
const uint8_t SOFT_MOSI_PIN = 21;
const uint8_t SOFT_SCK_PIN  = 33;

// SdFat software SPI template
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;
// Speed argument is ignored for software SPI.
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi)

MatrixPanel_I2S_DMA *dma_display = nullptr;

SdFs sd;

String gifDir = "/gifs"; // play all GIFs in this directory on the SD card
char filePath[256] = { 0 };
FsFile gifFile;

frame_status_t frame_state = STARTUP;
frame_status_t target_state = STARTUP;
frame_status_t lastState = frame_state;
unsigned long lastStateChange = 0;

void setup() {

//

  HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN    // Chain length
  );

  mxconfig.gpio.e = 18;
  mxconfig.clkphase = false;
  mxconfig.driver = HUB75_I2S_CFG::FM6126A;

  // Display Setup
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);
  dma_display->begin();
  dma_display->setBrightness8(90); //0-255
  dma_display->clearScreen();


  Serial.begin(115200);
  Serial.println("Starting AnimatedGIFs Sketch");

  // Start filesystem
  Serial.println(" * Loading SD");
  if(!sd.begin(SD_CONFIG)){
       sd.initErrorHalt();
        Serial.println("SD Mount Failed");
        return;
  }
  
dma_display->begin();
  
  /* all other pixel drawing functions can only be called after .begin() */
  dma_display->fillScreen(dma_display->color565(0, 0, 0));

  InitMatrixGif();
  	
    setupWifi();
  
}

// void handleGif()
// {
//    root = sd.open(gifDir);
//       if (root)
//       {
//            gifFile = root.openNextFile();
//             while (gifFile)
//             {
//               if (!gifFile.isDirectory()) // play it
//               {
                
//               char fileName[256];

//               gifFile.getName(fileName, 256);

//               String path = "/gifs/" + String(fileName);

//               Serial.print("Read file: ");
//               Serial.println(fileName);

//                 // C-strings... urghh...                
//                 memset(filePath, 0x0, sizeof(filePath));                
//                 strcpy(filePath, path.c_str());
                
//                 // Show it.
//                 ShowGIF(filePath);
               
//               }
//               gifFile.close();
//               gifFile = root.openNextFile();
//             }
//          root.close();
//       } // root
// }


void loop() 
{
    handleGifQueue();
    handleGif();
      
}