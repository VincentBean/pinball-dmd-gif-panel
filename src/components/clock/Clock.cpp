#include <Arduino.h>
#include <ezTime.h>
#include "Globals.h"
#include "Helpers.h"

void setupClock()
{
    message("Syncing clock", true);
    waitForSync();

    Timezone myTZ;
	myTZ.setLocation(F("Europe/Amsterdam"));

    delay(3000);

    message("Clock synced!", true);

    delay(2000);

    message(myTZ.dateTime("d-M-Y H:i:s"));
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