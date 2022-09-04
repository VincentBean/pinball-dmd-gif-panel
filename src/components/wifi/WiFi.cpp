#include <WiFiManager.h>
#include "Globals.h"
#include "Helpers.hpp"

WiFiManager wifiManager;

void configModeCallback (WiFiManager *myWiFiManager) {
    String ssid = myWiFiManager->getConfigPortalSSID();

    smallFont();
    message("WiFi Setup", true);
}

void setupWifi()
{
    if (!config.clockEnabled) {
        return;
    }

    WiFi.mode(WIFI_STA);
    
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setConfigPortalTimeout(180);

    wifiManager.setClass("invert");
    wifiManager.setScanDispPerc(true);

    wifiManager.setConnectRetries(10);
    wifiManager.setConnectTimeout(60);

    wifiManager.autoConnect("PinClock");
}