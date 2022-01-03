#include <WiFiManager.h>
#include "Globals.h"
#include "Helpers.hpp"

WiFiManager wifiManager;

void configModeCallback (WiFiManager *myWiFiManager) {
    String ssid = myWiFiManager->getConfigPortalSSID();

    message("WiFi Setup\nSSID: " + ssid, true);
}

void setupWifi()
{
    WiFi.mode(WIFI_STA);
    
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.autoConnect("DMD Clock");
}

void handleWifi()
{

}