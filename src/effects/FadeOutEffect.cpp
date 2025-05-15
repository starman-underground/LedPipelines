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
    if (this->running == DONE) return;

    if (this->running == NOT_STARTED) {
        this->startTimeMillis = millis();
        this->running = RUNNING;
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
        this->running = DONE;
        return;
    } else {
        this->running = RUNNING;
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