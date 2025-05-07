#pragma once

#include "BaseEffect.h"

namespace ledpipelines::effects {
class TimeBoxedEffect : public WrapperEffect, public TimedEffect {

public:
    TimeBoxedEffect(BaseLedPipelineStage *stage, float timeToRunSeconds);

    void reset() override;

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};

}