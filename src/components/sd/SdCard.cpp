#include <SdFat.h>

const uint8_t SD_CS_PIN = 22;
const uint8_t SOFT_MISO_PIN = 32;
const uint8_t SOFT_MOSI_PIN = 21;
const uint8_t SOFT_SCK_PIN = 33;

SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(0), &softSpi)

SdFs sd;

void displayDirectoryContent(FsFile& aDirectory, byte tabulation) 
{
  FsFile file;
  char fileName[20];

  if (!aDirectory.isDir()) return;
  aDirectory.rewind();

  while (file.openNext(&aDirectory, O_READ)) {
    if (!file.isHidden()) {
      file.getName(fileName, sizeof(fileName));
      for (uint8_t i = 0; i < tabulation; i++) Serial.write('\t');
      Serial.print(fileName);

      if (file.isDir()) {
        Serial.println(F("/"));
        displayDirectoryContent(file, tabulation + 1);
      } else {
        Serial.write('\t'); 
        file.printFileSize(&Serial);
        Serial.println(F(" bytes"));
      }
    }
    file.close();
  }
}

void InitSdCard()
{
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt();
    Serial.println("SD Mount Failed");
    return;
  }

  Serial.println("SD mounted!");
}