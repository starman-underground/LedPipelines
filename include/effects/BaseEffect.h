
#pragma once

#include "BaseLedPipeline.h"

class WrapperEffect : public BaseLedPipelineStage {
protected:
    BaseLedPipelineStage *stage;
public:
    explicit WrapperEffect(BaseLedPipelineStage *stage);

    ~WrapperEffect() override;

    void reset() override;
};


class LoopEffect : public WrapperEffect {

private:
    size_t numLoops = 0;
    size_t currentNumLoops = 0;

public:
    explicit LoopEffect(BaseLedPipelineStage *stage, size_t numLoops = 0);

    void calculate(int startIndex, TemporaryLedData tempData) override;

    void reset() override;
};

class RepeatEffect : public WrapperEffect {
private:
    size_t numRepeats = 1;
    size_t repeatDistance = 0;

public:
    RepeatEffect(BaseLedPipelineStage *stage, size_t numRepeats, size_t repeatDistance);

    void calculate(int startIndex, TemporaryLedData tempData) override;
};

class MovingEffect : public WrapperEffect {

private:
    float ledsPerSecond = 1;
    float currentPosition;
public:
    MovingEffect(BaseLedPipelineStage *stage, float ledsPerSecond);

    void calculate(int startIndex, TemporaryLedData tempData) override;
};