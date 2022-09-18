#include <Arduino.h>
#include <SdFat.h>
#include <AnimatedGIF.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "Globals.h"
#include "Helpers.hpp"
#include "components/config/Configuration.hpp"
#include "components/matrix/Matrix.hpp"
#include "components/gif/MatrixGif.hpp"
#include "components/gif/GifLoader.hpp"
#include "components/gif/GifPlayer.hpp"
#include "components/wifi/WiFi.hpp"
#include "components/clock/Clock.hpp"
#include "components/sd/SdCard.hpp"
#include "components/wifi/Webserver.hpp"
#include "components/update/FirmwareUpdate.hpp"

frame_status_t frame_state = PLAYING_ART;
frame_status_t target_state = PLAYING_ART;
frame_status_t lastState = frame_state;
unsigned long lastStateChange = 0;

TaskHandle_t scheduled_task;

void setup()
{
  Serial.begin(115200);

  InitMatrix();
  
  InitMatrixGif();
  
  InitSdCard();
  
  loadSettings();

  checkUpdate();

  setupWifi();

  InitWebserver();

  setupClock();
}

bool targetStateValid()
{
  if (frame_state == INDEXING && target_state != PLAYING_ART)
  {
    return false;
  }

  if (target_state == SHOW_TIME && !config.clockEnabled) 
  {
    return false;
  }

  return true;
}

void loop()
{
  if (sdError) {
    message("Check SD", true);
    delay(5000);
    clearMatrix();
    ESP.restart();
  }

  if (!gifPlaying && target_state != frame_state && targetStateValid())
  {
    frame_state = target_state;
    lastStateChange = millis();
  }

  handleGifQueue();

  if (frame_state == PLAYING_ART)
  {
    handleGif();
  }

  if (frame_state == SHOW_TIME)
  {
    handleClock();
  }

  if (saveConfig && !gifPlaying && frame_state != INDEXING)
  {
    saveSettings();
    saveConfig = false;
    Serial.println("Writing config");
  }
}