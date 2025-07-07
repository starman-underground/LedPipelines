#pragma once


#include "BaseEffect.h"


namespace ledpipelines::effects {
class MovingEffect : public WrapperEffect {

private:
    float ledsPerSecond;
    float currentPosition;
    float startPosition;
    float endPosition;
    float elapsedPercentage;
    unsigned long startTimeMillis;

public:
    MovingEffect(BaseLedPipelineStage *stage, float ledsPerSecond, float startPosition = 0,
                 float endPosition = TemporaryLedData::size);

    void calculate(float startIndex, TemporaryLedData &tempData) override;

    void reset() override;
};

}