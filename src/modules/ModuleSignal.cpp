#include "modules/ModuleSignal.h"

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

Adafruit_NeoPixel pixels(1, GPIO_NUM_33, NEO_GRB + NEO_KHZ800);

color_t ModuleSignal::pixelColor = COLOR____BLACK;

void ModuleSignal::begin() {
    pixels.begin();
}

void ModuleSignal::prepareSleep() {
    // gpio_hold_en((gpio_num_t)NEOPIXEL_POWER);
    gpio_hold_dis((gpio_num_t)NEOPIXEL_POWER);
}

color_t ModuleSignal::getPixelColor() {
    return ModuleSignal::pixelColor;
}

void ModuleSignal::setPixelColor(color_t pixelColor) {
    ModuleSignal::pixelColor = pixelColor;
    pixels.setPixelColor(0, ModuleSignal::pixelColor);  // red for measuring
    pixels.show();
}