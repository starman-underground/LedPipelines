#pragma once


#include "BaseEffect.h"

namespace ledpipelines::effects {

    class AbsolutePositionEffect : public WrapperEffect {

    public:
        int startPosition;

        explicit AbsolutePositionEffect(BaseLedPipelineStage *stage, int startPosition = 0);

        void calculate(int startIndex, TemporaryLedData &tempData) override;
    };
}