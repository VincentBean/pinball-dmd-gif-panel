#include <Arduino.h>
#include <ezTime.h>
#include "Globals.h"
#include "Helpers.hpp"

Timezone timezone;

void setupClock()
{
    if (!config.clockEnabled) {
        return;
    }

    setDebug(INFO);
    setServer();
    message("Syncing clock");
    
    waitForSync();
    
    message("Clock synced!", true);

	timezone.setLocation(config.timeZone);
    
    message("Timezone set");
}

void handleClock()
{
    if (!config.clockEnabled) {
        return;
    }

    events();
    
    bigFont();

    dma_display->fillScreen(dma_display->color565(0, 0, 0));

    dma_display->setTextColor(dma_display->color565(0, 255, 0));

    dma_display->println(timezone.dateTime("H:i:s"));
    delay(1000);
    target_state = PLAYING_ART;
}