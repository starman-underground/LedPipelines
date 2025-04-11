#pragma once

#include "BaseLedPipeline.h"

class SolidEffect : public BaseLedPipelineStage {
protected:
    CRGB color;

public:
    explicit SolidEffect(CRGB color);

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};


class SolidSegmentEffect : public SolidEffect {

protected:
    int segmentLength = 0;

public:
    explicit SolidSegmentEffect(CRGB color, int segmentLength);

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};