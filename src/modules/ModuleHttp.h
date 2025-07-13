#ifndef ModuleHttp_h
#define ModuleHttp_h

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "ModuleLed.h"

class ModuleHttp {
   private:
    static AsyncWebServer server;
    static bool hasBegun;

   public:
    static void begin();
    static void prepareSleep();
    static void handleCommandPower(AsyncWebServerRequest *request);
    static bool isNumeric(String value);
};

#endif