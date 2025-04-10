#include "FastLED.h"
#include "LedPipeline.h"

void setup() {
    CRGB *leds = new CRGB[1000];

    FastLED.addLeds<WS2812B, 11, GBR>(leds, 10);

    TemporaryLedData::initialize();

    delete[] leds;

}

void loop() {



}
