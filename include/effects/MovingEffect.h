#pragma once


#include "BaseEffect.h"



class MovingEffect : public WrapperEffect {

private:
    float ledsPerSecond = 1;
    float currentPosition;
public:
    MovingEffect(BaseLedPipelineStage *stage, float ledsPerSecond);

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};

class MovingSegmentEffect : public WrapperEffect {

private:
    float ledsPerSecond = 1;
    float currentPosition;
    int segmentLength;

public:
    MovingSegmentEffect(BaseLedPipelineStage *stage, float ledsPerSecond, int segmentLength);

    void calculate(int startIndex, TemporaryLedData &tempData) override;
};