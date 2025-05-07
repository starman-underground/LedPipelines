#pragma once


#include "BaseEffect.h"


namespace ledpipelines::effects {
    class OpacityScalingEffect : public WrapperEffect {

    public:
        uint8_t maxOpacity;

        OpacityScalingEffect(BaseLedPipelineStage *stage, uint8_t maxOpacity);

        void calculate(int startIndex, TemporaryLedData &tempData) override;

    };

}