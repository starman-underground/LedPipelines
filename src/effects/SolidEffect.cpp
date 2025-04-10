

#include "effects/SolidEffect.h"

SolidEffect::SolidEffect(CRGB color) : color(color) {}

void SolidEffect::calculate(int startIndex, TemporaryLedData tempData) {
    int numTotalLeds = FastLED.count();

    for (int i = startIndex; i < numTotalLeds; i++) {
        if (i < 0) continue;
        tempData[i] = color;
    }
}