#pragma once

#include "BaseEffect.h"


class StartFadeEffect : public WrapperEffect {
public:
    int fadeLength;
    LedPipelinesSmoothingType smoothingType;

    StartFadeEffect(
            BaseLedPipelineStage *stage,
            int fadeLength,
            LedPipelinesSmoothingType smoothingType = LedPipelinesSmoothingType::LINEAR
    );

    void calculate(int startIndex, TemporaryLedData &tempData) override;

};
