#include "modules/ModuleBattery.h"

#include <Arduino.h>

Adafruit_LC709203F ModuleBattery::baseBattery;

void ModuleBattery::begin() {
    ModuleBattery::baseBattery.begin();
    ModuleBattery::baseBattery.setPackSize(LC709203F_APA_3000MAH);
    ModuleBattery::baseBattery.setAlarmVoltage(3.8);
}

void ModuleBattery::prepareSleep() {
    // nothing
}

float ModuleBattery::getPercentage() {
    return ModuleBattery::baseBattery.cellPercent();
}
