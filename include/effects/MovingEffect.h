#pragma once


#include "BaseEffect.h"
#include "LedPipelineUtils.h"


namespace ledpipelines::effects {
class MovingEffect : public WrapperEffect {

private:
    float runtimeSeconds;
    float currentPosition;
    float startPosition;
    float endPosition;
    float elapsedPercentage;
    unsigned long startTimeMillis;
    SmoothingFunction smoothingFunction;

public:
    MovingEffect(BaseLedPipelineStage *stage, float runtimeSeconds, float startPosition = 0,
                 float endPosition = TemporaryLedData::size, SmoothingFunction smoothingFunction = SmoothingFunction::LINEAR);

    void calculate(float startIndex, TemporaryLedData &tempData) override;

    void reset() override;
};

}