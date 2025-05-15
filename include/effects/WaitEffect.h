#pragma once


#include "BaseEffect.h"
#include "LedPipelineUtils.h"

namespace ledpipelines::effects {
class WaitEffect : public BaseLedPipelineStage, TimedEffect {
public:

    WaitEffect(float waitTimeSeconds);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;

};


class RandomWaitEffect : public BaseLedPipelineStage, TimedEffect {
public:

    float minWaitTime;
    float maxWaitTime;

    SamplingFunction samplingFunction;

    RandomWaitEffect(float minWaitTime, float maxWaitTime,
                     SamplingFunction function = SamplingFunction::UNIFORM);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;
};
}