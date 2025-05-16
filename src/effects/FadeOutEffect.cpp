#include "effects/FadeOutEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

FadeOutEffect::FadeOutEffect(
        float fadeTime,
        SmoothingFunction function
) :
        TimedEffect(fadeTime),
        smoothingFunction(function) {}


void FadeOutEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->state == DONE) return;

    if (this->state == NOT_STARTED) {
        this->startTimeMillis = millis();
        this->state = RUNNING;
    }

    unsigned long currentTimeMillis = millis();

    float timeFadingSeconds = (currentTimeMillis - startTimeMillis) / 1000.0;

    // in this case, we have already finished fading, and can stop here.
    if (timeFadingSeconds >= timeToRunSeconds) {
        // when it's done, we still have to set it to done for the last frame.
        // so opacity is set to 0.
        for (int i = 0; i < TemporaryLedData::size; i++) {
            tempData.opacity[i] = 0;
        }
        elapsedPercentage = 1;
        this->state = DONE;
        return;
    } else {
        this->state = RUNNING;
        elapsedPercentage = timeFadingSeconds / timeToRunSeconds;
    }

    float opacityMultiplier = smoothingFunction(
            0,
            timeToRunSeconds,
            UINT8_MAX,
            0,
            timeFadingSeconds
    );

    for (int i = 0; i < TemporaryLedData::size; i++) {
        tempData.opacity[i] = (tempData.opacity[i] * opacityMultiplier) / 255;
    }
}

void FadeOutEffect::reset() {
    BaseLedPipelineStage::reset();
    TimedEffect::reset();
}

RandomFadeOutEffect::RandomFadeOutEffect(
        float minFadeTime,
        float maxFadeTime,
        SmoothingFunction smoothingFunction,
        SamplingFunction samplingFunction
) : RandomTimedEffect(minFadeTime, maxFadeTime, samplingFunction),
    smoothingFunction(smoothingFunction) {}

void RandomFadeOutEffect::calculate(int startIndex, ledpipelines::TemporaryLedData &tempData) {
    if (this->state == DONE) return;

    if (this->state == NOT_STARTED) {
        this->startTimeMillis = millis();
        this->sampleRuntime();
        this->state = RUNNING;
    }

    unsigned long currentTimeMillis = millis();

    float timeFadingSeconds = (currentTimeMillis - startTimeMillis) / 1000.0;

    // in this case, we have already finished fading, and can stop here.
    if (timeFadingSeconds >= timeToRunSeconds) {
        // when it's done, we still have to set it to done for the last frame.
        // so opacity is set to 0.
        for (int i = 0; i < TemporaryLedData::size; i++) {
            tempData.opacity[i] = 0;
        }
        elapsedPercentage = 1;
        this->state = DONE;
        return;
    } else {
        this->state = RUNNING;
        elapsedPercentage = timeFadingSeconds / timeToRunSeconds;
    }

    float opacityMultiplier = smoothingFunction(
            0,
            timeToRunSeconds,
            UINT8_MAX,
            0,
            timeFadingSeconds
    );

    for (int i = 0; i < TemporaryLedData::size; i++) {
        tempData.opacity[i] = (tempData.opacity[i] * opacityMultiplier) / 255;
    }
}


void RandomFadeOutEffect::reset() {
    RandomTimedEffect::reset();
}

