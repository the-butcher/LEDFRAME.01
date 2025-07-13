#ifndef ModuleWifi_h
#define ModuleWifi_h

#include <Arduino.h>
#include <WiFi.h>

const String WIFI_STA_KEY = "************";
const String WIFI_STA_PWD = "************";

class ModuleWifi {
   private:
    static void handleStationDisconnected(WiFiEvent_t event);

   public:
    static void begin();
    static void prepareSleep();
    static String getAddress();
};

#endif