
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

void ParallelLedPipeline::calculate(int startIndex, TemporaryLedData tempData) {
    if (!this->running)
        return;

    BaseLedPipelineStage *currentStage = firstStage;

    bool anyArePlaying = false;
    while (currentStage != nullptr) {
        TemporaryLedData currentStageData = TemporaryLedData();
        currentStage->calculate(startIndex, currentStageData);
        tempData.merge(currentStageData, currentStage->blendingMode);
        anyArePlaying = currentStage->running || anyArePlaying;
        currentStage = currentStage->nextStage;
    }

    this->running = anyArePlaying;
}


void SeriesLedPipeline::calculate(int startIndex, TemporaryLedData tempData) {
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

CRGB operator*(const CRGB first, const CRGB second) {
    return CRGB(
            ((size_t) first.r * (size_t) second.r) / 255,
            ((size_t) first.g * (size_t) second.g) / 255,
            ((size_t) first.b * (size_t) second.b) / 255
    );
}

CRGB &operator*=(CRGB &first, const CRGB &second) {
    first.r = ((size_t) first.r * (size_t) second.r) / 255;
    first.g = ((size_t) first.g * (size_t) second.g) / 255;
    first.b = ((size_t) first.b * (size_t) second.b) / 255;
    return first;
}

void TemporaryLedData::initialize() {
    // calculate the total number of LEDs.
    numLeds = 0;
    for (int i = 0; i < FastLED.count(); i++) {
        numLeds += FastLED[i].size();
    }
}

TemporaryLedData::TemporaryLedData() {
    numLeds = FastLED.count();
    data = new CRGB[numLeds];
    for (int i = 0; i < numLeds; i++) {
        (*this)[i] = CRGB::Black;
    }
}

TemporaryLedData::~TemporaryLedData() {
    free(data);
}

void TemporaryLedData::merge(TemporaryLedData &other, BlendingMode blendingMode) {

    for (int i = 0; i < TemporaryLedData::numLeds; i++) {
        switch (blendingMode) {
            case OVERWRITE:
                (*this)[i] = other[i];
                break;
            case ADD:
                (*this)[i] += other[i];
                break;
            case MULTIPLY:
                (*this)[i] *= other[i];
                break;
        }
    }
}

void TemporaryLedData::populateFastLed() {
    int currentLed = 0;
    for (int i = 0; i < FastLED.count(); i++) {
        for (int j = 0; j < FastLED[i].size(); (j++, currentLed++)) {
            FastLED[i][j] = (*this)[currentLed];
        }
    }
}
