#include "effects/LoopEffect.h"


LoopEffect::LoopEffect(BaseLedPipelineStage *stage, size_t numLoops) :
        WrapperEffect(stage),
        numLoops(numLoops),
        currentNumLoops(0) {}

void LoopEffect::reset() {
    WrapperEffect::reset();
    currentNumLoops = 0;
}

void LoopEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (running == DONE) {
        return;
    }

    if (this->running == NOT_STARTED)
        this->running = RUNNING;

    // run the current stage
    stage->calculate(startIndex, tempData);

    // check if the stage is done running. If still running, we return early.
    if (stage->running != DONE) {
        return;
    }

    // if the current stage is done running, we increase the number of completed loops.
    // if the number of completed loops is high enough, we stop running the effect.
    // if the number of loops to run is 0, we run the effect infinitely.
    currentNumLoops++;
    if (numLoops != 0 && currentNumLoops >= numLoops) {
        LPLogger::log("Current stage done, and loops completed.");
        this->running = DONE;
    } else {
        LPLogger::log(String("Current stage done, and looping again. Loops Completed: ")
                      + currentNumLoops + "/" + numLoops);
        this->running = RUNNING;
        this->stage->reset();
        // since the stage might not calculate anything in done, we need to redo the stage.
        this->stage->calculate(startIndex, tempData);
    }
}
