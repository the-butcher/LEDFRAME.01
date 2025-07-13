#ifndef ModuleBattery_h
#define ModuleBattery_h

#include <Adafruit_LC709203F.h>
#include <Arduino.h>

class ModuleBattery {
   private:
    static Adafruit_LC709203F baseBattery;

   public:
    static void begin();
    static void prepareSleep();
    static float getPercentage();
};

#endif