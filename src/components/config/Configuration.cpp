#include <ArduinoJson.h>
#include "Globals.h"
#include "Helpers.hpp"
#include "components/config/Configuration.hpp"
#include "components/gif/GifLoader.hpp"

#define CONFIG_FILENAME "/config.json"
#define CONFIG_SIZE 512

Config config;
bool saveConfig;

#define CLOCK_ENABLED_KEY "clock_enabled"
#define CLOCK_R_KEY "clock_r"
#define CLOCK_G_KEY "clock_g"
#define CLOCK_B_KEY "clock_b"
#define SHOW_TIME_EVERY_KEY "show_time_every_x_gifs"
#define MIN_GIF_SHOW_TIME_KEY "min_gif_show_time"
#define BRIGHTNESS_KEY "brightness"
#define GIF_LOAD_STRATEGY_KEY "gif_load_strategy"
#define TIMEZONE_KEY "timezone"

void applySettings()
{
  dma_display->setBrightness8(config.brightness);
}

void loadSettings()
{
  FsFile configFile = sd.open(CONFIG_FILENAME, FILE_READ);

  StaticJsonDocument<CONFIG_SIZE> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  if (error)
  {
    Serial.println("Using default configuration");
    delay(500);  
  }
  
  config.clockEnabled = doc[CLOCK_ENABLED_KEY] | false;
  config.clockR = doc[CLOCK_R_KEY] | 231;
  config.clockG = doc[CLOCK_G_KEY] | 103;
  config.clockB = doc[CLOCK_B_KEY] | 3;
  config.showTimeEveryXGif = doc[SHOW_TIME_EVERY_KEY] | 5;
  config.minGifShowtime = doc[MIN_GIF_SHOW_TIME_KEY] | 2000;

  config.brightness = doc[BRIGHTNESS_KEY] | 80;
  config.timeZone = String(doc[TIMEZONE_KEY] | "Europe/Amsterdam");
  
  config.loadStrategy = doc[GIF_LOAD_STRATEGY_KEY] | SEQUENTIAL;

  if (configFile) 
  {
    configFile.close();
  }

  applySettings();  
  saveSettings();
}

void saveSettings()
{
  StaticJsonDocument<CONFIG_SIZE> doc;

  doc[CLOCK_ENABLED_KEY] = config.clockEnabled;
  doc[CLOCK_R_KEY] = config.clockR;
  doc[CLOCK_G_KEY] = config.clockR;
  doc[CLOCK_B_KEY] = config.clockR;

  doc[SHOW_TIME_EVERY_KEY] = config.showTimeEveryXGif;
  doc[MIN_GIF_SHOW_TIME_KEY] = config.minGifShowtime;

  doc[BRIGHTNESS_KEY] = config.brightness;
  doc[GIF_LOAD_STRATEGY_KEY] = config.loadStrategy;
  doc[TIMEZONE_KEY] = config.timeZone;

  if (sd.exists(CONFIG_FILENAME))
  {
    sd.remove(CONFIG_FILENAME);
  }

  FsFile configFile = sd.open(CONFIG_FILENAME, FILE_WRITE);

  if (!configFile)
  {
    Serial.println(F("Failed to create file"));
    return;
  }

  if (serializeJson(doc, configFile) == 0)
  {
    Serial.println(F("Failed to write to file"));
  }

  configFile.close();
}