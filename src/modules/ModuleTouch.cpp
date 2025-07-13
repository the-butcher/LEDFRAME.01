#include "modules/ModuleTouch.h"

#include "ModuleLed.h"
#include "ModuleMqtt.h"

boolean ModuleTouch::isTouchUp = false;
boolean ModuleTouch::isTouchDn = false;

void ModuleTouch::begin() {
    // 12 increases power
    // 11 decreases power
}

void ModuleTouch::prepareSleep() {
    touchSleepWakeUpEnable(PIN_____TOUCH___UP, THRESHOLD / 6);
}

void ModuleTouch::loop() {
    int valueUp = touchRead(PIN_____TOUCH___UP);
    if (valueUp >= THRESHOLD && !isTouchUp) {
        isTouchUp = true;
        power_t power = (power_t)(ModuleLed::getPower() + 1);
        ModuleLed::setPower(power, MINUTES_TO_AUTO_OFF_TOUCH);
        ModuleMqtt::publishState();
    } else if (valueUp < THRESHOLD && isTouchUp) {
        isTouchUp = false;
    }
    int valueDn = touchRead(PIN_____TOUCH___DN);
    if (valueDn >= THRESHOLD && !isTouchDn) {
        isTouchDn = true;
        power_t power = (power_t)(ModuleLed::getPower() - 1);
        ModuleLed::setPower(power, MINUTES_TO_AUTO_OFF_TOUCH);
        ModuleMqtt::publishState();
    } else if (valueDn < THRESHOLD && isTouchDn) {
        isTouchDn = false;
    }
}
