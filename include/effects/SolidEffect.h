#pragma once

#include "BaseLedPipeline.h"

namespace ledpipelines::effects {
class SolidEffect : public BaseLedPipelineStage {
protected:
    CRGB color;
    uint8_t opacity;

public:
    explicit SolidEffect(CRGB color, uint8_t opacity = 255);

    void calculate(float startIndex, TemporaryLedData &tempData) override;
};


class SolidSegmentEffect : public SolidEffect {

protected:
    float segmentLength = 0;

public:
    explicit SolidSegmentEffect(CRGB color, float segmentLength, uint8_t opacity = 255);

    void calculate(float startIndex, TemporaryLedData &tempData) override;
};

}