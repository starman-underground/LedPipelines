#pragma once

#include "BaseEffect.h"


class TimeBoxedEffect : public WrapperEffect {

public:

    unsigned long startTimeMs;

    float timeToRunSeconds;

    float elapsedPercentage;

    TimeBoxedEffect(BaseLedPipelineStage *stage, float timeToRunSeconds);

    void reset() override;

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};