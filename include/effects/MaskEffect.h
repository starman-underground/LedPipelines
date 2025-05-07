#pragma once


#include "BaseEffect.h"


namespace ledpipelines::effects {
    class MaskEffect : public BaseLedPipelineStage {

    public:
        BaseLedPipelineStage *mask;
        BaseLedPipelineStage *base;
        bool startFullOpacity;

        MaskEffect(
                BaseLedPipelineStage *base,
                BaseLedPipelineStage *mask,
                bool startFullOpacity = false
        );

        void calculate(int startIndex, TemporaryLedData &tempData) override;

        void reset() override;
    };

}
