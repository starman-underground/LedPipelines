#pragma once

#include "BaseLedPipeline.h"

class SolidEffect : public BaseLedPipelineStage {
protected:
    CRGB color;

public:
    SolidEffect(CRGB color);

    void calculate(int startIndex, TemporaryLedData tempData) override;
};