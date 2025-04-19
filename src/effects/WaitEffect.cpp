#include "effects/WaitEffect.h"


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

    this->elapsedPercentage = ((float) totalTimeWaited / 1000.0) / this->timeToRunSeconds; // convert ms to seconds
    this->running = RUNNING;
}


void WaitEffect::reset() {
    BaseLedPipelineStage::reset();
    TimedEffect::reset();
}