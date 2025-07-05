#pragma once


#include "BaseEffect.h"

namespace ledpipelines::effects {
class FadeInEffect : public BaseLedPipelineStage, TimedEffect {

public:
    SmoothingFunction smoothingFunction;

    FadeInEffect(
            float fadeTime,
            SmoothingFunction smoothingFunction = SmoothingFunction::SMOOTH_LINEAR
    );

    void calculate(float startIndex, TemporaryLedData &tempData) override;

    void reset() override;
};

class RandomFadeInEffect : public BaseLedPipelineStage, RandomTimedEffect {
public:
    SmoothingFunction smoothingFunction;

    RandomFadeInEffect(
            float minFadeTime,
            float maxFadeTime,
            SmoothingFunction smoothingFunction = SmoothingFunction::SMOOTH_LINEAR,
            SamplingFunction samplingFunction = SamplingFunction::UNIFORM
            );

    void calculate(float startIndex, TemporaryLedData &tempData) override;

    void reset() override;
};

}