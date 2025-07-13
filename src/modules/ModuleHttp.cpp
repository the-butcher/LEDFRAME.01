#include "ModuleHttp.h"

#include <ArduinoJson.h>

#include "modules/ModuleMqtt.h"
#include "modules/ModuleWifi.h"

AsyncWebServer ModuleHttp::server(80);
bool ModuleHttp::hasBegun = false;

void ModuleHttp::begin() {
    if (!ModuleHttp::hasBegun) {
        server.on("/cmnd/power", HTTP_GET, handleCommandPower);
        DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
        server.begin();
        ModuleHttp::hasBegun = true;
    }
}

void ModuleHttp::prepareSleep() {
    server.end();
}

/**
 * get details about device status
 */
void ModuleHttp::handleCommandPower(AsyncWebServerRequest *request) {

    AsyncResponseStream *response = request->beginResponseStream("application/json");
    response->addHeader("Cache-Control", "max-age=60");
    JsonDocument jsonDocument;
    jsonDocument["code"] = 200;
    int minutesToAutoOff = 1;
    if (request->hasParam("m")) {
        String m = request->getParam("m")->value();
        if (ModuleHttp::isNumeric(m)) {
            minutesToAutoOff = m.toInt();
            jsonDocument["m"] = minutesToAutoOff;
        }
    }
    if (request->hasParam("p")) {
        String p = request->getParam("p")->value();
        if (p == "0") {
            ModuleLed::setPower(POWER__OFF, minutesToAutoOff);
            ModuleMqtt::publishState();
            jsonDocument["p"] = POWER__OFF;
        } else if (p == "1") {
            ModuleLed::setPower(POWER__LOW, minutesToAutoOff);
            ModuleMqtt::publishState();
            jsonDocument["p"] = POWER__LOW;
        } else if (p == "2") {
            ModuleLed::setPower(POWER__MID, minutesToAutoOff);
            ModuleMqtt::publishState();
            jsonDocument["p"] = POWER__MID;
        } else if (p == "3") {
            ModuleLed::setPower(POWER_HIGH, minutesToAutoOff);
            ModuleMqtt::publishState();
            jsonDocument["p"] = POWER_HIGH;
        } else {
            jsonDocument["code"] = 400;
            jsonDocument["p"] = "p must be one of [0, 1, 2, 3]";
        }
    } else {
        jsonDocument["code"] = 400;
        jsonDocument["p"] = "p must be present";
    }
    // if (request->hasParam("t")) {
    //     // TODO :: implement timeout for turning off
    //     jsonDocument["t"] = request->getParam("t")->value();
    // }

    serializeJson(jsonDocument, *response);
    request->send(response);
}

bool ModuleHttp::isNumeric(String value) {
    for (uint8_t i = 0; i < value.length(); i++) {
        if (!isDigit(value.charAt(i))) {
            return false;
        }
    }
    return true;
}
