#pragma once


#include "BaseEffect.h"


namespace ledpipelines::effects {
class FadeOutEffect : public BaseLedPipelineStage, TimedEffect {

public:
    SmoothingFunction smoothingFunction;

    FadeOutEffect(float fadeTime, SmoothingFunction function = SmoothingFunction::LINEAR);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;


};

class RandomFadeOutEffect : public BaseLedPipelineStage, RandomTimedEffect {

public:
    SmoothingFunction smoothingFunction;

    RandomFadeOutEffect(
            float minFadeTime,
            float maxFadeTime,
            SmoothingFunction smoothingFunction = SmoothingFunction::LINEAR,
            SamplingFunction samplingFunction = SamplingFunction::UNIFORM
    );

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;

};

}