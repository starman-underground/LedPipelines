


#include "effects/StartFadeEffect.h"


StartFadeEffect::StartFadeEffect(BaseLedPipelineStage *stage, int fadeLength, LedPipelinesSmoothingType smoothingType)
        : WrapperEffect(stage), fadeLength(fadeLength), smoothingType(smoothingType) {

}


void StartFadeEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE)
        return;

    if (this->running == NOT_STARTED) {
        this->running = RUNNING;
    }

    TemporaryLedData stageData = TemporaryLedData();

    this->stage->calculate(0, stageData);

    for (int i = 0; i < fadeLength; i++) {
        stageData.opacity[i] = interpolate(
                smoothingType,
                0,
                fadeLength,
                0,
                stageData.opacity[i],
                i
        );
        LPLogger::log(tempData.opacity[i]);
    }

    for (int i = 0; i < TemporaryLedData::size; i++) {
        tempData.set(i + startIndex, stageData[i], stageData.opacity[i]);
    }


    this->running = this->stage->running;
}