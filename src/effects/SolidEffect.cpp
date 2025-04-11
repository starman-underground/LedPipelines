

#include "effects/SolidEffect.h"

SolidEffect::SolidEffect(CRGB color) : BaseLedPipelineStage(OVERWRITE), color(color) {
    LedPipelinesUtils::log(LOG, String("solid created with blending mode ") + blendingMode);

}

void SolidEffect::calculate(int startIndex, TemporaryLedData &tempData) {

    for (int i = startIndex; i < TemporaryLedData::size; i++) {
        if (i < 0) continue;
        tempData.set(i, color);
    }
}


SolidSegmentEffect::SolidSegmentEffect(CRGB color, int segmentLength)
        : SolidEffect(color),
          segmentLength(segmentLength) {
    LedPipelinesUtils::log(LOG, String("segment created with blending mode ") + blendingMode);
}

void SolidSegmentEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    for (int i = startIndex; i < startIndex + segmentLength && i < TemporaryLedData::size; i++) {
        tempData.set(i, color);
    }
}