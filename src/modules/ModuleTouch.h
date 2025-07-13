#ifndef ModuleTouch_h
#define ModuleTouch_h

#include <Arduino.h>

const gpio_num_t PIN_____TOUCH___DN = GPIO_NUM_9;
const gpio_num_t PIN_____TOUCH___UP = GPIO_NUM_10;
const touch_value_t THRESHOLD = 30000;
const int MINUTES_TO_AUTO_OFF_TOUCH = 2;

class ModuleTouch {
   private:
    static boolean isTouchUp;
    static boolean isTouchDn;

   public:
    static void begin();
    static void loop();
    static void prepareSleep();
};

#endif