#include <Arduino.h>
#include <ezTime.h>
#include "Globals.h"

void setupClock()
{
    setDebug(INFO);

    waitForSync();

    Timezone myTZ;
	myTZ.setLocation(F("Europe/Amsterdam"));

    delay(5000);

    	Serial.println(myTZ.dateTime());

}

void handleClock()
{
   events();

   dma_display->fillScreen(dma_display->color565(0, 0, 0));

    dma_display->setCursor(0, 0);
    dma_display->setTextSize(1);
    dma_display->setTextColor(dma_display->color565(0, 255, 0));

    dma_display->println(dateTime("d-M-Y H:i:s"));
    delay(1000);
    target_state = PLAYING_ART;
}