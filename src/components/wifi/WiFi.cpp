#include <WiFiManager.h>
#include "Globals.h"

WiFiManager wifiManager;


void configModeCallback (WiFiManager *myWiFiManager) {
    String ssid = myWiFiManager->getConfigPortalSSID();

    dma_display->setCursor(0, 0);
    dma_display->setTextSize(1);
    dma_display->setTextColor(dma_display->color565(0, 255, 0));

    dma_display->println("WiFi Setup");
    dma_display->println("SSID: " + ssid);
}

void setupWifi()
{
    dma_display->setCursor(0, 0);
    dma_display->setTextSize(1);
    dma_display->setTextColor(dma_display->color565(0, 255, 0));

    dma_display->println("Connecting");

    WiFi.mode(WIFI_STA);
    
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.autoConnect("DMD Clock");

}

void handleWifi()
{

}