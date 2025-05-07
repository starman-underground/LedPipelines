#pragma once



#include "BaseEffect.h"

namespace ledpipelines::effects {
    class WaitEffect : public BaseLedPipelineStage, TimedEffect {
    public:

        WaitEffect(float waitTimeSeconds);

        void calculate(int startIndex, TemporaryLedData &tempData) override;

        void reset() override;

    };
}