#pragma once

#include "BaseEffect.h"


namespace ledpipelines::effects {
class OpacityGradientEffect : public WrapperEffect {
public:
    float fadeLength;
    float startIndex;
    SmoothingFunction smoothingFunction;

    OpacityGradientEffect(BaseLedPipelineStage *stage, float fadeLength, float startIndex = 0,
                          SmoothingFunction smoothingType = SmoothingFunction::SMOOTH_LINEAR);

    void calculate(float startIndex, TemporaryLedData &tempData) override;


private:
    void calculateForwardGradient(float startIndex, TemporaryLedData &tempData);

    void calculateBackwardGradient(float startIndex, TemporaryLedData &tempData);
};

}