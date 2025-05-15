#pragma once


#include "BaseEffect.h"


namespace ledpipelines::effects {
class FadeOutEffect : public BaseLedPipelineStage, TimedEffect {

public:
    float fadeTime;
    SmoothingFunction smoothingFunction;

    FadeOutEffect(float fadeTime, SmoothingFunction function = SmoothingFunction::LINEAR);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;


};

}