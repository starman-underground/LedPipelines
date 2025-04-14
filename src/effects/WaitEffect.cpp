#include "effects/WaitEffect.h"



WaitEffect::WaitEffect(float waitTimeSeconds) : waitTimeSeconds(waitTimeSeconds) {}

void WaitEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE) return;

    if (this->running == NOT_STARTED) this->running = RUNNING;

    unsigned long totalTimeWaited = millis() - this->startTimeMs;

    if (totalTimeWaited / 1000.0 >= waitTimeSeconds) {
        timeWaitingPercentage = 1;
        this->running = DONE;
        return;
    }

    this->timeWaitingPercentage = ((float) totalTimeWaited / 1000.0) / waitTimeSeconds; // convert ms to seconds
    this->running = RUNNING;
}



void WaitEffect::reset() {
    BaseLedPipelineStage::reset();
    this->timeWaitingPercentage = 0;
    this->startTimeMs = millis();
}