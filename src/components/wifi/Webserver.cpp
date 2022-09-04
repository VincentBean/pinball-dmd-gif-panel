#include <ESPAsyncWebServer.h>
#include "Globals.h"

AsyncWebServer *server;

void InitWebserver()
{
    if (!config.clockEnabled) {
        return;
    }

    server = new AsyncWebServer(80);

    server->on("/brightness", HTTP_POST, [](AsyncWebServerRequest *request)
               {
                   Serial.println("set panel brightness");

                   if (!request->hasParam("value"))
                   {
                       request->send(400, "text/plain", "Missing parameter: value");
                       return;
                   }

                   int value = atoi(request->getParam("value")->value().c_str());

                   if (value <= 1 || value >= 255)
                   {
                       request->send(400, "invalid value");
                       return;
                   }

                   config.brightness = value;
                   saveConfig = true;

                   request->send(200, "text/plain", String(config.brightness));
               });

    server->on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, "text/plain", String(config.brightness)); });

     server->on("/restart", HTTP_GET, [](AsyncWebServerRequest *request)
               { request->send(200, "text/plain", "OK"); ESP.restart(); });

    server->begin();
}