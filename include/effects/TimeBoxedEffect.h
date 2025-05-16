#pragma once

#include "BaseEffect.h"

namespace ledpipelines::effects {
class TimeBoxedEffect : public WrapperEffect, public TimedEffect {

public:
    TimeBoxedEffect(BaseLedPipelineStage *stage, float timeToRunSeconds);

    void reset() override;

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};

class RandomTimeBoxedEffect : public WrapperEffect, public RandomTimedEffect {

public:

    RandomTimeBoxedEffect(
            BaseLedPipelineStage *stage,
            float maxRuntime,
            SamplingFunction samplingFunction
    );

    RandomTimeBoxedEffect(
            BaseLedPipelineStage *stage,
            float minRuntime,
            float maxRuntime,
            SamplingFunction samplingFunction
    );

    void reset() override;

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};


}