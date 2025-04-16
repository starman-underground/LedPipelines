

#include "effects/SolidEffect.h"

SolidEffect::SolidEffect(CRGB color) : BaseLedPipelineStage(NORMAL), color(color) {
    this->running = RUNNING;
}

void SolidEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    for (int i = startIndex; i < TemporaryLedData::size; i++) {
        tempData.set(i, color);
    }
}


SolidSegmentEffect::SolidSegmentEffect(CRGB color, int segmentLength)
        : SolidEffect(color),
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