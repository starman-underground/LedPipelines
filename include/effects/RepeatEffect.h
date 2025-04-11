#pragma once

#include "BaseEffect.h"

class RepeatEffect : public WrapperEffect {
private:
    int numRepeats = 1;
    int repeatDistance = 0;

public:
    RepeatEffect(BaseLedPipelineStage *stage, int numRepeats, int repeatDistance);

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};
