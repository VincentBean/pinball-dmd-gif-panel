#include <Arduino.h>
#include <SdFat.h>
#include <AnimatedGIF.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Globals.h"
#include "components/gif/MatrixGif.hpp"
#include "components/gif/GifLoader.hpp"
#include "components/gif/GifPlayer.hpp"
#include "components/wifi/WiFi.hpp"
#include "components/clock/Clock.hpp"

#define PANEL_RES_X 64 // Number of pixels wide of each INDIVIDUAL panel module.
#define PANEL_RES_Y 32 // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_CHAIN 2  // Total number of panels chained one to another

const uint8_t SD_CS_PIN = 22;
//
// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = 32;
const uint8_t SOFT_MOSI_PIN = 21;
const uint8_t SOFT_SCK_PIN = 33;

// SdFat software SPI template
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;
// Speed argument is ignored for software SPI.
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi)

MatrixPanel_I2S_DMA *dma_display = nullptr;

SdFs sd;

String gifDir = "/gifs"; // play all GIFs in this directory on the SD card
char filePath[256] = {0};
FsFile gifFile;

frame_status_t frame_state = STARTUP;
frame_status_t target_state = STARTUP;
frame_status_t lastState = frame_state;
unsigned long lastStateChange = 0;

TaskHandle_t scheduled_task;

void handleScheduled(void *param)
{

  for (;;)
  {
    handleGifQueue();

    vTaskDelay(1 / portTICK_PERIOD_MS); // https://github.com/espressif/esp-idf/issues/1646#issue-299097720
  }
}

void setup()
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

  Serial.begin(115200);
  Serial.println("Starting AnimatedGIFs Sketch");

  // Start filesystem
  Serial.println(" * Loading SD");
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt();
    Serial.println("SD Mount Failed");
    return;
  }

  // xTaskCreatePinnedToCore(
  //     handleScheduled, /* Function to implement the task */
  //     "schedule",      /* Name of the task */
  //     10000,           /* Stack size in words */
  //     NULL,            /* Task input parameter */
  //     0,               /* Priority of the task */
  //     &scheduled_task, /* Task handle. */
  //     0);              /* Core where the task should run */

  dma_display->begin();

  dma_display->fillScreen(dma_display->color565(0, 0, 0));

  InitMatrixGif();

  setupWifi();

  setupClock();
}

bool targetStateValid()
{
  if (frame_state == INDEXING && target_state != PLAYING_ART)
  {
    return false;
  }

  return true;
}


void loop()
{
  if (!gifPlaying && target_state != frame_state && targetStateValid())
  {
    frame_state = target_state;
    lastStateChange = millis();

    Serial.print("new state: ");
    Serial.println(frame_state);
  }

  handleGifQueue();

  if (frame_state == STARTUP || frame_state == PLAYING_ART)
  {
    handleGif();
  }

  if (frame_state == SHOW_TIME)
  {
    handleClock();
  }

}