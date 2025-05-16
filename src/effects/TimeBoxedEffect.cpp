#include "effects/TimeBoxedEffect.h"
#include "LedPipelinesLogger.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

TimeBoxedEffect::TimeBoxedEffect(BaseLedPipelineStage *stage, float timeToRunSeconds)
        : WrapperEffect(stage), TimedEffect(timeToRunSeconds) {}


void TimeBoxedEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE) return;

    if (this->running == NOT_STARTED) {
        LPLogger::log(String("starting time boxed effect. Running for ") + timeToRunSeconds + " seconds");
        this->running = RUNNING;
        startTimeMillis = millis();
    }

    unsigned long elapsedTime = millis() - startTimeMillis;
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
    TimedEffect::reset();
}


RandomTimeBoxedEffect::RandomTimeBoxedEffect(
        ledpipelines::BaseLedPipelineStage *stage,
        float maxRuntime,
        ledpipelines::SamplingFunction samplingFunction
) : RandomTimeBoxedEffect(stage, 0, maxRuntime, samplingFunction) {}

RandomTimeBoxedEffect::RandomTimeBoxedEffect(
        ledpipelines::BaseLedPipelineStage *stage,
        float minRuntime,
        float maxRuntime,
        ledpipelines::SamplingFunction samplingFunction
) : WrapperEffect(stage), RandomTimedEffect(minRuntime, maxRuntime, samplingFunction) {}

void RandomTimeBoxedEffect::calculate(int startIndex, ledpipelines::TemporaryLedData &tempData) {
    if (this->running == DONE) return;

    if (this->running == NOT_STARTED) {
        LPLogger::log(String("starting random time boxed effect. Running for ") + timeToRunSeconds + " seconds");
        this->running = RUNNING;
        this->sampleRuntime();
        startTimeMillis = millis();
    }

    unsigned long elapsedTime = millis() - startTimeMillis;
    if (elapsedTime / 1000.0 >= timeToRunSeconds) {
        LPLogger::log("done running random time boxed effect.");
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


void RandomTimeBoxedEffect::reset() {
    WrapperEffect::reset();
    RandomTimedEffect::reset();
}






















