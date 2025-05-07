#include "effects/SolidEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

SolidEffect::SolidEffect(CRGB color, uint8_t opacity) :
        BaseLedPipelineStage(NORMAL),
        color(color),
        opacity(opacity) {
    this->running = RUNNING;
}

void SolidEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    for (int i = startIndex; i < TemporaryLedData::size; i++) {
        tempData.set(i, color, opacity);
    }
}


SolidSegmentEffect::SolidSegmentEffect(CRGB color, int segmentLength, uint8_t opacity)
        : SolidEffect(color, opacity),
          segmentLength(segmentLength) {}

void SolidSegmentEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (segmentLength > 0) {
        for (int i = startIndex; i < startIndex + segmentLength; i++) {
            tempData.set(i, color);
        }
    } else {
        for (int i = startIndex; i > startIndex - segmentLength; i--) {
            tempData.set(i, color);
        }
    }
}