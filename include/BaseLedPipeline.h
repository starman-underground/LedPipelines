#pragma once

#include "FastLED.h"
#include "LedPipelineUtils.h"

class BaseLedPipelineStage {
public:

    bool running = true;

    BlendingMode blendingMode = ADD;

    BaseLedPipelineStage *nextStage = nullptr;

    virtual void calculate(int startIndex, TemporaryLedData &tempData) = 0;

    virtual void reset();

    explicit BaseLedPipelineStage(BlendingMode blendingMode = ADD);

    virtual ~BaseLedPipelineStage();
};


/**
 * Base abstract class for defining LED pipelines - the construct that can be run
 * and show effects on the
 */
class LedPipeline : public BaseLedPipelineStage {

public:

    virtual LedPipeline *addStage(BaseLedPipelineStage *stage);

    virtual void run();

    void reset() override;

    ~LedPipeline() override;

protected:
    /**
     * linked list of first and last stages. used when adding stages and running them.
     */
    BaseLedPipelineStage *firstStage = nullptr;
    BaseLedPipelineStage *lastStage = nullptr;

};


/**
 * A pipeline that runs all its stages in parallel. The stage that gets added first
 * gets applied first, with other layers getting applied on top. The pipeline continues
 * until all stages have completed.
 */
class ParallelLedPipeline : public LedPipeline {

public:

    void calculate(int startIndex, TemporaryLedData &tempData) override;

};

/**
 * A pipeline that blocks every stage before it moves on to the next one. The stage must be considered complete (the
 * calculate function must return false) before it moves on to the next stage. This is useful for functions that need
 * to happen one after another, not at the same time.
 */
class SeriesLedPipeline : public LedPipeline {

public:
    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;

private:
    BaseLedPipelineStage *currentStage = nullptr;
};



