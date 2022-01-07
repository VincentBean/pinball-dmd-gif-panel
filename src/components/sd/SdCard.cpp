#include <SdFat.h>
#include "Helpers.hpp"

const uint8_t SD_CS_PIN = 22;
const uint8_t SOFT_MISO_PIN = 32;
const uint8_t SOFT_MOSI_PIN = 21;
const uint8_t SOFT_SCK_PIN = 33;

SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi)

SdFs sd;

void InitSdCard()
{
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt();
    message("SD fail", true);
    return;
  }

  Serial.println("SD mounted!");
}