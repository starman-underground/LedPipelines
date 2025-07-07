#pragma once


#include "BaseEffect.h"

namespace ledpipelines::effects {

class AbsolutePositionEffect : public WrapperEffect {

public:
    int startPosition;

    explicit AbsolutePositionEffect(BaseLedPipelineStage *stage, float startPosition = 0);

    void calculate(float startIndex, TemporaryLedData &tempData) override;
};
}