


#include "effects/OpacityScalingEffect.h"


OpacityScalingEffect::OpacityScalingEffect(BaseLedPipelineStage *stage, uint8_t maxOpacity)
        : WrapperEffect(stage), maxOpacity(maxOpacity) {}

void OpacityScalingEffect::calculate(int startIndex, TemporaryLedData &tempData) {

    if (this->running == DONE)
        return;

    if (this->running == NOT_STARTED)
        this->running = RUNNING;

    this->stage->calculate(startIndex, tempData);
    for (int i = 0; i < tempData.size; i++) {
        tempData.opacity[i] = (tempData.opacity[i] * maxOpacity) / 255;
    }

    this->running = this->stage->running;
}
