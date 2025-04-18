#pragma once


#include "BaseEffect.h"

class FadeInEffect : public WrapperEffect, TimedEffect {

public:
    float startOffsetSeconds;
    float fadeTime;
    LedPipelinesSmoothingType smoothingType;

    FadeInEffect(
            BaseLedPipelineStage *stage,
            float fadeTime,
            float startOffsetSeconds = 0,
            LedPipelinesSmoothingType smoothingType = SMOOTH_LINEAR);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;


};