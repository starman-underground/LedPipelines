
#include "BaseLedPipeline.h"


BaseLedPipelineStage::BaseLedPipelineStage(BlendingMode blendingMode)
        : blendingMode(blendingMode),
          running(true) {}

BaseLedPipelineStage::~BaseLedPipelineStage() {
    delete nextStage;
}

void BaseLedPipelineStage::reset() {
    if (!this->running)
        this->running = true;
}


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
    if (!this->running) {
        return;
    }
    FastLED.clear();
    TemporaryLedData data = TemporaryLedData();
    this->calculate(0, data);
    data.populateFastLed();
    FastLED.show();
}

void LedPipeline::reset() {
    BaseLedPipelineStage *currentStage = firstStage;
    while (currentStage != nullptr) {
        currentStage->reset();
        currentStage = currentStage->nextStage;
    }
}

void ParallelLedPipeline::calculate(int startIndex, TemporaryLedData &tempData) {
    if (!this->running)
        return;

    BaseLedPipelineStage *currentStage = firstStage;

    if (currentStage == nullptr) {
        LedPipelinesUtils::log(WARN, "no stages in the pipeline.");
    }

    bool anyArePlaying = false;
    int currentStageNumber = 0;
    while (currentStage != nullptr) {
        TemporaryLedData currentStageData = TemporaryLedData();
        currentStage->calculate(startIndex, currentStageData);
        tempData.merge(currentStageData, currentStage->blendingMode);
        anyArePlaying = currentStage->running || anyArePlaying;
        currentStage = currentStage->nextStage;
        currentStageNumber++;
    }

    this->running = anyArePlaying;

}


void SeriesLedPipeline::calculate(int startIndex, TemporaryLedData &tempData) {
    if (!this->running)
        return;

    // this is the first time we are running, so currentStage hasn't been set to the first stage yet.
    if (currentStage == nullptr) {
        currentStage = firstStage;
    }

    // there are no stages in the pipeline. return early.
    if (currentStage == nullptr) {
        this->running = false;
        return;
    }

    // run the current stage. If the current stage is done running, we set the current stage to the next stage.
    currentStage->calculate(startIndex, tempData);
    if (!currentStage->running) {
        currentStage = currentStage->nextStage;
    }

    // if the next stage is null, we know that the pipeline is done running. We can set running to false.
    this->running = (currentStage != nullptr);
}

void SeriesLedPipeline::reset() {
    LedPipeline::reset();
    this->currentStage = nullptr;
}


