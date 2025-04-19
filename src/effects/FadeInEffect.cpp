#include "effects/FadeInEffect.h"


FadeInEffect::FadeInEffect(
        float fadeTime,
        LedPipelinesSmoothingType smoothingType
) :
        TimedEffect(fadeTime),
        smoothingType(smoothingType) {}


void FadeInEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE) return;

    if (this->running == NOT_STARTED) {
        this->startTimeMillis = millis();
        this->running = RUNNING;
    }

    unsigned long currentTimeMillis = millis();

    float timeFadingSeconds = (currentTimeMillis - startTimeMillis) / 1000.0;

    // in this case, we have already finished fading, and can stop here.
    if (timeFadingSeconds >= timeToRunSeconds) {
        elapsedPercentage = 1;
        this->running = DONE;
        return;
    } else {
        this->running = RUNNING;
        elapsedPercentage = timeFadingSeconds / timeToRunSeconds;
    }

    float opacityMultiplier = interpolate(
            smoothingType,
            0,
            timeToRunSeconds,
            0,
            UINT8_MAX,
            timeFadingSeconds
    );

    for (int i = 0; i < TemporaryLedData::size; i++) {
        tempData.opacity[i] = (tempData.opacity[i] * opacityMultiplier) / 255;
    }
}

void FadeInEffect::reset() {
    BaseLedPipelineStage::reset();
    TimedEffect::reset();
}