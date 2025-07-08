#pragma once

#include "BaseEffect.h"

namespace ledpipelines::effects {
class RepeatEffect : public WrapperEffect {
private:
    int numRepeats = 1;
    float repeatDistance = 0;

public:
    RepeatEffect(BaseLedPipelineStage *stage, float repeatDistance, int numRepeats = 0);

    void calculate(float startIndex, TemporaryLedData &tempData) override;
};

}