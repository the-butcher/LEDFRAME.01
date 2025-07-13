#include "ModuleWifi.h"

#include <ArduinoJson.h>

// #include "driver/adc.h"

void ModuleWifi::begin() {

    WiFi.persistent(true);
    // WiFi.onEvent(ModuleWifi::handleStationDisconnected, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_STA_KEY, WIFI_STA_PWD);
    for (int i = 0; i < 20; i++) {
        delay(100);
        if (WiFi.isConnected()) {
            break;
        };  // WiFi.isConnected() includes both WIFI_MODE_STA and WIFI_MODE_APSTA
    }
}

void ModuleWifi::handleStationDisconnected(WiFiEvent_t event) {
    // nothing
}

void ModuleWifi::prepareSleep() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

String ModuleWifi::getAddress() {
    wifi_mode_t wifiMode = WiFi.getMode();
    if (wifiMode == WIFI_STA) {
        return WiFi.localIP().toString();
    } else if (wifiMode == WIFI_OFF) {
        return "wifi off";
    } else {
        return String(wifiMode);
    }
}
