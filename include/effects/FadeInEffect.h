#pragma once


#include "BaseEffect.h"

namespace ledpipelines::effects {
class FadeInEffect : public BaseLedPipelineStage, TimedEffect {

public:
    float fadeTime;
    SmoothingFunction smoothingFunction;

    FadeInEffect(
            float fadeTime,
            SmoothingFunction function = SmoothingFunction::SMOOTH_LINEAR
    );

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;


};

}