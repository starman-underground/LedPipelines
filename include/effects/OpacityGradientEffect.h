#pragma once

#include "BaseEffect.h"


class OpacityGradientEffect : public WrapperEffect {
public:
    int fadeLength;
    int startIndex;
    LedPipelinesSmoothingType smoothingType;

    OpacityGradientEffect(BaseLedPipelineStage *stage, int fadeLength, int startIndex = 0,
                          LedPipelinesSmoothingType smoothingType = LedPipelinesSmoothingType::SMOOTH_LINEAR);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

};
