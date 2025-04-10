#include "effects/BaseEffect.h"


WrapperEffect::WrapperEffect(BaseLedPipelineStage *stage) :
        stage(stage) {}

WrapperEffect::~WrapperEffect() {
    delete stage;
}

void WrapperEffect::reset() {
    BaseLedPipelineStage::reset();
    stage->reset();
}



LoopEffect::LoopEffect(BaseLedPipelineStage *stage, size_t numLoops) :
        WrapperEffect(stage),
        numLoops(numLoops),
        currentNumLoops(0) {}

void LoopEffect::reset() {
    WrapperEffect::reset();
    currentNumLoops = 0;
}

void LoopEffect::calculate(int startIndex, TemporaryLedData tempData) {
    if (!running) {
        return;
    }

    // run the current stage
    stage->calculate(startIndex, tempData);

    // check if the stage is done running. If still running, we return early.
    if (stage->running) {
        return;
    }

    // if the current stage is done running, we increase the number of completed loops.
    // if the number of completed loops is high enough, we stop running the effect.
    // if the number of loops to run is 0, we run the effect infinitely.
    currentNumLoops++;
    if (numLoops != 0 && currentNumLoops >= numLoops) {
        this->running = false;
    } else {
        this->running = true;
        this->stage->reset();
    }
}

