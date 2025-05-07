
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

}





