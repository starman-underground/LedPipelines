#pragma once


#include "BaseEffect.h"


namespace ledpipelines::effects {
class MovingEffect : public WrapperEffect {

private:
    float ledsPerSecond;
    float currentPosition;
    int startPosition;
    int endPosition;
    float elapsedPercentage;
    unsigned long startTimeMillis;

public:
    MovingEffect(BaseLedPipelineStage *stage, float ledsPerSecond, int startPosition = 0,
                 int endPosition = TemporaryLedData::size);

    void calculate(float startIndex, TemporaryLedData &tempData) override;

    void reset() override;
};

}