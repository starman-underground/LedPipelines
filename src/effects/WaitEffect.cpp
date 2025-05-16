#include <utility>

#include "effects/WaitEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;


WaitEffect::WaitEffect(float waitTimeSeconds) : TimedEffect(waitTimeSeconds) {}

void WaitEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE) return;

    if (this->running == NOT_STARTED) {
        this->running = RUNNING;
        this->startTimeMillis = millis();
    }

    unsigned long totalTimeWaited = millis() - this->startTimeMillis;

    if (totalTimeWaited / 1000.0 >= this->timeToRunSeconds) {
        this->elapsedPercentage = 1;
        this->running = DONE;
        return;
    }

    this->elapsedPercentage = ((float) totalTimeWaited / 1000.0f) / this->timeToRunSeconds; // convert ms to seconds
    this->running = RUNNING;
}


void WaitEffect::reset() {
    BaseLedPipelineStage::reset();
    TimedEffect::reset();
}


RandomWaitEffect::RandomWaitEffect(float maxWaitTime, const ledpipelines::SamplingFunction &function)
        : RandomWaitEffect(0,
                           maxWaitTime,
                           function) {}

RandomWaitEffect::RandomWaitEffect(float minWaitTime, float maxWaitTime, const SamplingFunction &function)
        : BaseLedPipelineStage(),
          RandomTimedEffect(minWaitTime, maxWaitTime, function) {}


void RandomWaitEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE) {
        return;
    }

    if (this->running == NOT_STARTED) {
        this->running = RUNNING;
        this->startTimeMillis = millis();
        this->sampleRuntime();
    }

    unsigned long totalTimeWaited = millis() - this->startTimeMillis;

    if (totalTimeWaited / 1000.0 >= this->timeToRunSeconds) {
        this->elapsedPercentage = 1;
        this->running = DONE;
        return;
    }

    this->elapsedPercentage = ((float) totalTimeWaited / 1000.0f) / this->timeToRunSeconds; // convert ms to seconds
    this->running = RUNNING;
}

void RandomWaitEffect::reset() {
    BaseLedPipelineStage::reset();
    RandomTimedEffect::reset();
}


