#pragma once


#include "BaseEffect.h"
#include "LedPipelineUtils.h"


namespace ledpipelines::effects {
class MovingEffect : public WrapperEffect {

private:
    float runtimeSeconds;
    // float currentPosition;
    std::pair<float, float> startPosition;
    std::pair<float, float> endPosition;
    float elapsedPercentage;
    unsigned long startTimeMillis;
    LedLayout layout;
    SmoothingFunction smoothingFunction;

public:
    MovingEffect(BaseLedPipelineStage *stage, float runtimeSeconds, float startPosition = 0,
                 float endPosition = TemporaryLedData::size, SmoothingFunction smoothingFunction = SmoothingFunction::LINEAR,
                 LedLayout layout = LedLayout(LedLayout::ONE_DIMENSIONAL, TemporaryLedData::size, 0));
    MovingEffect(BaseLedPipelineStage *stage, float runtimeSeconds, LedLayout layout,
                 std::pair<float, float> startPosition, std::pair<float, float> endPosition,
                 SmoothingFunction smoothingFunction = SmoothingFunction::LINEAR);

    void calculate(float startIndex, TemporaryLedData &tempData) override;
    void calculate(std::pair<float, float> start, TemporaryLedData &tempData);

    void reset() override;
};

}