#include <Arduino.h>
#include <SdFat.h>
#include <AnimatedGIF.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Globals.h"
#include "Helpers.hpp"
#include "components/matrix/Matrix.hpp"
#include "components/gif/MatrixGif.hpp"
#include "components/gif/GifLoader.hpp"
#include "components/gif/GifPlayer.hpp"
#include "components/wifi/WiFi.hpp"
#include "components/clock/Clock.hpp"
#include "components/sd/SdCard.hpp"

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
  Serial.begin(115200);

  message("Initializing");

  InitMatrix();

  message("Init SD", true);

  InitSdCard();

  message("Init GIF", true);

  // xTaskCreatePinnedToCore(
  //     handleScheduled, /* Function to implement the task */
  //     "schedule",      /* Name of the task */
  //     10000,           /* Stack size in words */
  //     NULL,            /* Task input parameter */
  //     0,               /* Priority of the task */
  //     &scheduled_task, /* Task handle. */
  //     0);              /* Core where the task should run */

  InitMatrixGif();

  message("Init WiFi", true);

  setupWifi();

  message("Init Clock", true);

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