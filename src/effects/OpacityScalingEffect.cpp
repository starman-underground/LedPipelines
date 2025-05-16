#include "effects/OpacityScalingEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

OpacityScalingEffect::OpacityScalingEffect(BaseLedPipelineStage *stage, uint8_t maxOpacity)
        : WrapperEffect(stage), maxOpacity(maxOpacity) {}

void OpacityScalingEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->state == DONE)
        return;

    if (this->state == NOT_STARTED)
        this->state = RUNNING;

    LPLogger::log(String("starting opacity scaling with max opacity ") + maxOpacity);
    this->stage->calculate(startIndex, tempData);
    tempData.printData();
    for (int i = 0; i < tempData.size; i++) {
        tempData.opacity[i] = (tempData.opacity[i] * maxOpacity) / 255;
    }
    tempData.printData();
    LPLogger::log("done with opacity scaling");


    this->state = this->stage->state;
}
