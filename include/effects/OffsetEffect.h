#pragma once


#include "BaseEffect.h"

namespace ledpipelines::effects {
class OffsetEffect : public WrapperEffect {

public:
    int offset;

public:
    OffsetEffect(BaseLedPipelineStage *stage, float offset);

    void calculate(float startIndex, TemporaryLedData &tempData) override;
};

}