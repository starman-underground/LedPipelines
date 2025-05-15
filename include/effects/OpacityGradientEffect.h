#pragma once

#include "BaseEffect.h"


namespace ledpipelines::effects {
class OpacityGradientEffect : public WrapperEffect {
public:
    int fadeLength;
    int startIndex;
    SmoothingFunction smoothingFunction;

    OpacityGradientEffect(BaseLedPipelineStage *stage, int fadeLength, int startIndex = 0,
                          SmoothingFunction smoothingType = SmoothingFunction::SMOOTH_LINEAR);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

};

}