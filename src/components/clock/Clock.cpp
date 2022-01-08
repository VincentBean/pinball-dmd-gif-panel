#include <Arduino.h>
#include <ezTime.h>
#include "Globals.h"
#include "Helpers.hpp"

Timezone timezone;

void setupClock()
{
    setDebug(INFO);
    setServer("10.0.0.4");
    message("Syncing clock", true);
    
    waitForSync();
    
    message("Clock synced!", true);

	timezone.setLocation(config.timeZone);
    
    message("Timezone set", true);
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