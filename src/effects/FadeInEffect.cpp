#include "effects/FadeInEffect.h"


FadeInEffect::FadeInEffect(
        BaseLedPipelineStage *stage,
        float fadeTime,
        float startOffsetSeconds,
        LedPipelinesSmoothingType smoothingType
) :
        WrapperEffect(stage),
        TimedEffect(fadeTime),
        startOffsetSeconds(startOffsetSeconds),
        smoothingType(smoothingType) {}


void FadeInEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE) return;

    if (this->running == NOT_STARTED) {
        this->startTimeMillis = millis();
        this->running = RUNNING;
    }

    unsigned long currentTimeMillis = millis();

    // if we haven't waited for the start effect to complete, then we have to finish
    // waiting for the effect to complete.
    if ((currentTimeMillis - startTimeMillis) <= startOffsetSeconds * 1000) {

    }



}

void FadeInEffect::reset() {
    WrapperEffect::reset();
    TimedEffect::reset();
}