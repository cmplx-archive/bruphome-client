#ifndef NEOPIXELTESTHDR
#define NEOPIXELTESTHDR

#include "../Device.h"

class NeoPixelTest : public Device {

    void registerAllEvents();
    void testHandler(JsonObjectConst data, JsonObject result);

    void init(JsonObjectConst data, JsonObject result);

    void LEDOn(JsonObjectConst data, JsonObject result);
    void AllOn(JsonObjectConst data, JsonObject result);
    void AllOff(JsonObjectConst data, JsonObject result);
    int HexToInt(String data);
    void AllRainbow();
    
    Task* TaskAllRainbow;
	

};

#endif