
#pragma once

#include "BaseLedPipeline.h"

namespace ledpipelines::effects {
class WrapperEffect : public BaseLedPipelineStage {
protected:
    BaseLedPipelineStage *stage;
public:
    explicit WrapperEffect(BaseLedPipelineStage *stage);

    ~WrapperEffect() override;

    void reset() override;
};


class TimedEffect {
protected:
    unsigned long startTimeMillis;
    float elapsedPercentage;
    float timeToRunSeconds;

    TimedEffect(float timeToRunSeconds);

    void reset();
};


class RandomTimedEffect : public TimedEffect {
protected:
    float minRuntime;
    float maxRuntime;
    SamplingFunction samplingFunction;

    RandomTimedEffect(
            float maxRuntime,
            SamplingFunction samplingFunction = SamplingFunction::UNIFORM
    );

    RandomTimedEffect(
            float minRuntime,
            float maxRuntime,
            SamplingFunction samplingFunction = SamplingFunction::UNIFORM
    );

    void sampleRuntime();

    void reset();
};

}





