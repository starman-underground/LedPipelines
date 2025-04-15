
#include "BaseLedPipeline.h"


BaseLedPipelineStage::BaseLedPipelineStage(BlendingMode blendingMode)
        : blendingMode(blendingMode),
          running(NOT_STARTED) {}

BaseLedPipelineStage::~BaseLedPipelineStage() {
    delete nextStage;
}

void BaseLedPipelineStage::reset() {
    this->running = NOT_STARTED;
}

LedPipeline::LedPipeline(BlendingMode mode) : BaseLedPipelineStage(mode) {}

LedPipeline::~LedPipeline() {
    delete firstStage;
}

LedPipeline *LedPipeline::addStage(BaseLedPipelineStage *stage) {
    if (this->firstStage == nullptr) {
        this->firstStage = stage;
        this->lastStage = stage;
    } else {
        this->lastStage->nextStage = stage;
        this->lastStage = stage;
    }
    return this;
}


void LedPipeline::run() {
    if (this->running == DONE) {
        return;
    }
    FastLED.clear();
    TemporaryLedData data = TemporaryLedData();
    this->calculate(0, data);
    data.populateFastLed();
    FastLED.show();
}

void LedPipeline::reset() {
    BaseLedPipelineStage::reset();
    BaseLedPipelineStage *currentStage = firstStage;
    while (currentStage != nullptr) {
        currentStage->reset();
        currentStage = currentStage->nextStage;
    }
}

ParallelLedPipeline::ParallelLedPipeline(BlendingMode mode) : LedPipeline(mode) {}

void ParallelLedPipeline::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE)
        return;

    if (this->running == NOT_STARTED)
        this->running = RUNNING;

    BaseLedPipelineStage *currentStage = firstStage;

    if (currentStage == nullptr) {
        LPLogger::warn("no stages in the pipeline.");
    }

    LedPipelineRunningState anyArePlaying = DONE;
    int currentStageNumber = 0;
    while (currentStage != nullptr) {
//        LPLogger::log(String("Running stage: ") + currentStageNumber);
        TemporaryLedData currentStageData = TemporaryLedData();
        currentStage->calculate(startIndex, currentStageData);
//        currentStageData.printData();
        tempData.merge(currentStageData, currentStage->blendingMode);
//        tempData.printData();
        anyArePlaying = currentStage->running == RUNNING ? RUNNING : anyArePlaying;
        currentStage = currentStage->nextStage;
        currentStageNumber++;
    }

    this->running = anyArePlaying;
}

SeriesLedPipeline::SeriesLedPipeline(BlendingMode mode) : LedPipeline(mode) {}

void SeriesLedPipeline::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE)
        return;

    // this is the first time we are running, so currentStage hasn't been set to the first stage yet.
    if (this->running == NOT_STARTED) {
        LPLogger::log("starting series LED pipeline.");
        currentStage = firstStage;
        this->running = RUNNING;
    }

    // there are no stages in the pipeline. return early.
    if (currentStage == nullptr) {
        LPLogger::warn("No stages in series pipeline. Exiting early.");
        this->running = DONE;
        return;
    }

    // run the current stage. If the current stage is done running, we set the current stage to the next stage.
    currentStage->calculate(startIndex, tempData);
    if (currentStage->running == DONE) {
        currentStage = currentStage->nextStage;
    }

    // if the current stage is null, we know that the pipeline is done running. We can set running to false.
    if (currentStage == nullptr) {
        this->running = DONE;
        LPLogger::log("Series pipeline is done running.");
    }
}

void SeriesLedPipeline::reset() {
    LedPipeline::reset();
    this->currentStage = nullptr;
}


