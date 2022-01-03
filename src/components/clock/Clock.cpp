#include <Arduino.h>
#include <ezTime.h>
#include "Globals.h"
#include "Helpers.hpp"

Timezone timezone;

void setupClock()
{
    message("Syncing clock", true);
    waitForSync();

	timezone.setLocation(F("Europe/Amsterdam"));

    delay(2000);

    message("Clock synced!", true);

    delay(1000);
}

void handleClock()
{
    events();
    
    bigFont();

    dma_display->fillScreen(dma_display->color565(0, 0, 0));

    dma_display->setTextColor(dma_display->color565(0, 255, 0));

    dma_display->println(timezone.dateTime("H:i:s"));
    delay(1000);
    target_state = PLAYING_ART;
}