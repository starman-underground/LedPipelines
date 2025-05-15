#pragma once

#include "BaseEffect.h"


namespace ledpipelines::effects {
class LoopEffect : public WrapperEffect {

private:
    size_t numLoops = 0;
    size_t currentNumLoops = 0;

public:
    explicit LoopEffect(BaseLedPipelineStage *stage, size_t numLoops = 0);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;
};

}