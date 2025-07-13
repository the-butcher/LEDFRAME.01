#include <Arduino.h>

#include "modules/ModuleBattery.h"
#include "modules/ModuleHttp.h"
#include "modules/ModuleMqtt.h"
#include "modules/ModuleSignal.h"
#include "modules/ModuleTouch.h"
#include "modules/ModuleWifi.h"

const int MICROSECONDS_PER_SECOND = 1000000;

void initializeDeepSleep() {
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
    esp_sleep_enable_timer_wakeup(30 * MICROSECONDS_PER_SECOND);
    ModuleSignal::setPixelColor(COLOR_____BLUE);
    ModuleLed::prepareSleep();
    ModuleTouch::prepareSleep();
    ModuleMqtt::prepareSleep();
    ModuleHttp::prepareSleep();
    ModuleWifi::prepareSleep();
    ModuleSignal::prepareSleep();
    ModuleBattery::prepareSleep();  // nothing
    esp_deep_sleep_start();
}

void setup() {

    Serial.begin(115200);

    ModuleBattery::begin();

    ModuleSignal::begin();
    ModuleSignal::setPixelColor(COLOR____GREEN);

    ModuleLed::begin();
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    // if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER)
    if (wakeup_reason == ESP_SLEEP_WAKEUP_TOUCHPAD) {
        ModuleLed::setPower(POWER__LOW, MINUTES_TO_AUTO_OFF_TOUCH);
    }

    ModuleWifi::begin();
    ModuleHttp::begin();
    ModuleMqtt::begin();
    ModuleTouch::begin();

    if (wakeup_reason == ESP_SLEEP_WAKEUP_TOUCHPAD) {
        ModuleMqtt::setPropagatePower(false);
    } else {
        ModuleMqtt::setPropagatePower(true);
    }

    ModuleSignal::setPixelColor(COLOR____BLACK);
}

void loop() {
    for (uint8_t i = 0; i < 10; i++) {
        ModuleMqtt::loop();
        ModuleTouch::loop();
        delay(100);
    }
    ModuleLed::loop();  // should be enough to check for auto off every second only
    if (!ModuleMqtt::isPropagatePower()) {
        ModuleMqtt::publishState();  // late publish for touch power on
    }
    ModuleMqtt::setPropagatePower(true);
    if (ModuleLed::getPower() == POWER__OFF) {
        initializeDeepSleep();
    }
}
