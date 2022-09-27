#include <WiFiManager.h>
#include "Globals.h"
#include "Helpers.hpp"

#define CLEAR_SETTINGS_FLAG "/reset_creds"

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

    if (sd.exists(CLEAR_SETTINGS_FLAG))
    {
        wifiManager.resetSettings();
        sd.remove(CLEAR_SETTINGS_FLAG);
    }

    WiFi.mode(WIFI_STA);
    
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setConfigPortalTimeout(180);

    wifiManager.setScanDispPerc(true);

    wifiManager.setConnectRetries(3);
    wifiManager.setConnectTimeout(2);

    wifiManager.autoConnect("DmdPlayer");
}