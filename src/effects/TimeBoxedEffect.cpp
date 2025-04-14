#include "effects/TimeBoxedEffect.h"


TimeBoxedEffect::TimeBoxedEffect(BaseLedPipelineStage *stage, float timeToRunSeconds)
        : WrapperEffect(stage), timeToRunSeconds(timeToRunSeconds) {}


void TimeBoxedEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE) return;

    if (this->running == NOT_STARTED) {
        LPLogger::log(String("starting time boxed effect. Running for ") + timeToRunSeconds + " seconds");
        this->running = RUNNING;
        startTimeMs = millis();
    }

    unsigned long elapsedTime = millis() - startTimeMs;
    if (elapsedTime / 1000.0 >= timeToRunSeconds) {
        LPLogger::log("done running time boxed effect.");
        this->elapsedPercentage = 1;
        this->running = DONE;
        return;
    }

    this->elapsedPercentage = elapsedTime / 1000.0f / timeToRunSeconds;
    this->stage->calculate(startIndex, tempData);
    // if the internal stage is done, we set it to done.
    if (this->stage->running == DONE) {
        this->running = DONE;
    }
}


void TimeBoxedEffect::reset() {
    WrapperEffect::reset();
    this->startTimeMs = millis();
    this->elapsedPercentage = 0;
}