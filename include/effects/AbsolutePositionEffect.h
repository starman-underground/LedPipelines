#pragma once


#include "BaseEffect.h"

namespace ledpipelines::effects {

class AbsolutePositionEffect : public WrapperEffect {

public:
    int startPosition;

    explicit AbsolutePositionEffect(BaseLedPipelineStage *stage, float startPosition = 0);
    explicit AbsolutePositionEffect(BaseLedPipelineStage *stage, float startX, float startY, LedLayout layout)
        : AbsolutePositionEffect(stage, layout.calculateLedIndex(startX, startY)) {}

    void calculate(float startIndex, TemporaryLedData &tempData) override;
};
}