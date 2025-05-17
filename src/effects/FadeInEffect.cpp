#include "effects/FadeInEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

FadeInEffect::FadeInEffect(
        float fadeTime,
        SmoothingFunction smoothingFunction
) :
        TimedEffect(fadeTime),
        smoothingFunction(smoothingFunction) {}


void FadeInEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->state == LedPipelineRunningState::DONE) return;

    if (this->state == LedPipelineRunningState::NOT_STARTED) {
        this->startTimeMillis = millis();
        this->state =  LedPipelineRunningState::RUNNING;
    }

    unsigned long currentTimeMillis = millis();

    float timeFadingSeconds = (currentTimeMillis - startTimeMillis) / 1000.0;

    // in this case, we have already finished fading, and can stop here.
    if (timeFadingSeconds >= timeToRunSeconds) {
        elapsedPercentage = 1;
        // when it's done, we still have to set it to done for the last frame.
        // so opacity is set to 255.
        for (int i = 0; i < TemporaryLedData::size; i++) {
            tempData.opacity[i] = UINT8_MAX;
        }
        this->state = LedPipelineRunningState::DONE;
        return;
    } else {
        this->state =  LedPipelineRunningState::RUNNING;
        elapsedPercentage = timeFadingSeconds / timeToRunSeconds;
    }

    float opacityMultiplier = smoothingFunction(
            timeFadingSeconds,
            0,
            timeToRunSeconds,
            0,
            UINT8_MAX
    );

    for (int i = 0; i < TemporaryLedData::size; i++) {
        tempData.opacity[i] = opacityMultiplier * 255;
    }
}

void FadeInEffect::reset() {
    BaseLedPipelineStage::reset();
    TimedEffect::reset();
}

RandomFadeInEffect::RandomFadeInEffect(
        float minFadeTime,
        float maxFadeTime,
        SmoothingFunction smoothingFunction,
        SamplingFunction samplingFunction
) : RandomTimedEffect(minFadeTime, maxFadeTime, samplingFunction),
    smoothingFunction(smoothingFunction) {}


void RandomFadeInEffect::calculate(int startIndex, ledpipelines::TemporaryLedData &tempData) {
    if (this->state == LedPipelineRunningState::DONE) return;

    if (this->state == LedPipelineRunningState::NOT_STARTED) {
        this->startTimeMillis = millis();
        this->sampleRuntime();
        LPLogger::log(String("running random fade in effect for ") + this->timeToRunSeconds + " seconds");
        this->state =  LedPipelineRunningState::RUNNING;
    }

    unsigned long currentTimeMillis = millis();

    float timeFadingSeconds = (currentTimeMillis - startTimeMillis) / 1000.0;

    // in this case, we have already finished fading, and can stop here.
    if (timeFadingSeconds >= timeToRunSeconds) {
        LPLogger::log("done running random fade in effect.");
        elapsedPercentage = 1;
        // when it's done, we still have to set it to done for the last frame.
        // so opacity is set to 255.
        for (int i = 0; i < TemporaryLedData::size; i++) {
            tempData.opacity[i] = UINT8_MAX;
        }
        this->state = LedPipelineRunningState::DONE;
        return;
    } else {
        this->state =  LedPipelineRunningState::RUNNING;
        elapsedPercentage = timeFadingSeconds / timeToRunSeconds;
    }

    float currentOpacity = smoothingFunction(
            timeFadingSeconds,
            0,
            timeToRunSeconds,
            0,
            UINT8_MAX
    );

    for (int i = 0; i < TemporaryLedData::size; i++) {
        tempData.opacity[i] = currentOpacity;
    }
}

void RandomFadeInEffect::reset() {
    BaseLedPipelineStage::reset();
    RandomTimedEffect::reset();
}
