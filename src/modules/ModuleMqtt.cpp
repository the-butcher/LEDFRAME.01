#include "ModuleMqtt.h"

#include <WiFiClientSecure.h>

#include "ModuleBattery.h"
#include "ModuleLed.h"
#include "ModuleWifi.h"

WiFiClient* ModuleMqtt::wifiClient;
PubSubClient* ModuleMqtt::mqttClient;
boolean ModuleMqtt::propagatePower = false;

void ModuleMqtt::begin() {
    ModuleMqtt::wifiClient = new WiFiClient();
    ModuleMqtt::mqttClient = new PubSubClient(*ModuleMqtt::wifiClient);
    ModuleMqtt::mqttClient->setServer(MQTT_SRV.c_str(), MQTT_PRT);
    ModuleMqtt::mqttClient->setCallback(ModuleMqtt::handleMessage);
}

void ModuleMqtt::connect() {
    // Serial.println("connecting mqttClient");
    if (MQTT_KEY != "" && MQTT_PWD != "") {
        mqttClient->connect(MQTT_CLI.c_str(), MQTT_KEY.c_str(), MQTT_PWD.c_str(), 0, 0, 0, 0, 0);
    } else {
        mqttClient->connect(MQTT_CLI.c_str());  // connect without credentials
    }
    ModuleMqtt::mqttClient->subscribe(MQTT_SUB.c_str());  // MQTT_SUB.c_str()
}

void ModuleMqtt::loop() {
    if (!ModuleMqtt::mqttClient->connected()) {
        ModuleMqtt::connect();
    }
    mqttClient->loop();
}

void ModuleMqtt::prepareSleep() {
    ModuleMqtt::mqttClient->disconnect();            // calls stop() on wificlient
    while (ModuleMqtt::mqttClient->state() != -1) {  // https://github.com/knolleary/pubsubclient/issues/452
        delay(10);                                   // wait for disconnect (will flush the last message, if present)
    }
    ModuleMqtt::wifiClient->stop();  // explicit stop to be sure it happened (before resetting certFileData)
    delete ModuleMqtt::mqttClient;   // releases some memory buffer
    delete ModuleMqtt::wifiClient;   // calls stop (again) and deletes an internal sslclient instance
    ModuleMqtt::mqttClient = NULL;
    ModuleMqtt::wifiClient = NULL;
}

void ModuleMqtt::setPropagatePower(boolean isPropagatePower) {
    ModuleMqtt::propagatePower = isPropagatePower;
}

boolean ModuleMqtt::isPropagatePower() {
    return ModuleMqtt::propagatePower;
}

mqtt____stat__e ModuleMqtt::getClientState() {
    int state = ModuleMqtt::mqttClient->state();
    if (state == MQTT_CONNECTION_TIMEOUT) {  // -4
        return MQTT_TIMEOUT____CONN;
    } else if (state == MQTT_CONNECTION_LOST) {  // -3
        return MQTT_LOST_______CONN;
    } else if (state == MQTT_CONNECT_FAILED) {  // -2
        return MQTT_FAIL_______CONN;
    } else if (state == MQTT_DISCONNECTED) {  // -1
        return MQTT_LOST_______CONN;
    } else if (state == MQTT_CONNECTED) {  // 0 :: OK
        return MQTT______________OK;
    } else if (state == MQTT_CONNECT_BAD_PROTOCOL) {  // 1
        return MQTT_BAD____PROTOCOL;
    } else if (state == MQTT_CONNECT_BAD_CLIENT_ID) {  // 2
        return MQTT_BAD_________CLI;
    } else if (state == MQTT_CONNECT_UNAVAILABLE) {  // 3
        return MQTT_UNAVAIL____CONN;
    } else if (state == MQTT_CONNECT_BAD_CREDENTIALS) {  // 4
        return MQTT_BAD_CREDENTIALS;
    } else if (state == MQTT_CONNECT_UNAUTHORIZED) {  // 5
        return MQTT_NO_________AUTH;
    } else {
        return MQTT_________UNKNOWN;
    }
}

void ModuleMqtt::handleMessage(char* topic, byte* payload, unsigned int length) {
    if (MQTT_SUB == String(topic)) {
        JsonDocument doc;
        deserializeJson(doc, payload, length);
        int minutesToAutoOff = 1;
        if (!doc["m"].isNull() && doc["m"].is<int>()) {
            minutesToAutoOff = doc["m"].as<int>();
        }
        if (!doc["p"].isNull() && doc["p"].is<int>()) {
            int p = doc["p"].as<int>();
            if (p >= POWER__OFF && p <= POWER_HIGH && ModuleMqtt::propagatePower) {
                ModuleLed::setPower((power_t)p, minutesToAutoOff);
            }
            ModuleMqtt::publishState();
            ModuleMqtt::clearCommand();
        }
    }
}

void ModuleMqtt::publishState() {

    if (!ModuleMqtt::mqttClient->connected()) {
        ModuleMqtt::connect();
    }

    JsonDocument jsonDocument;
    jsonDocument["lp"] = ModuleLed::getPower();
    jsonDocument["bp"] = ModuleBattery::getPercentage();
    jsonDocument["ip"] = ModuleWifi::getAddress();

    // https://arduinojson.org/v7/how-to/use-arduinojson-with-pubsubclient/
    char outputBuf[128];
    serializeJson(jsonDocument, outputBuf);
    ModuleMqtt::mqttClient->publish(MQTT_PUB.c_str(), outputBuf, true);
}

void ModuleMqtt::clearCommand() {

    if (!ModuleMqtt::mqttClient->connected()) {
        ModuleMqtt::connect();
    }

    char outputBuf[0];
    ModuleMqtt::mqttClient->publish(MQTT_SUB.c_str(), outputBuf, true);
}