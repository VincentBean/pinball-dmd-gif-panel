#include <ArduinoJson.h>
#include "Globals.h"
#include "Helpers.hpp"
#include "components/config/Configuration.hpp"
#include "components/gif/GifLoader.hpp"

#define CONFIG_FILENAME "/config.json"
#define CONFIG_SIZE 512

Config config;
bool saveConfig;

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
    message("Default config");
    delay(500);  
  }
  
  config.brightness = doc[BRIGHTNESS_KEY] | 80;
  config.timeZone = String(doc[TIMEZONE_KEY] | "Europe/Amsterdam");
  
  config.loadStrategy = doc[GIF_LOAD_STRATEGY_KEY] | INDEXED;
  //config.loadStrategy = doc[GIF_LOAD_STRATEGY_KEY] | SEQUENTIAL;

  if (configFile) 
  {
    configFile.close();
  }

  applySettings();  
}

void saveSettings()
{
  StaticJsonDocument<CONFIG_SIZE> doc;

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