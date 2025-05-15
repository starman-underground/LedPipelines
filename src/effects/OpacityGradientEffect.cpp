#include "effects/OpacityGradientEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

OpacityGradientEffect::OpacityGradientEffect(BaseLedPipelineStage *stage, int fadeLength, int startIndex,
                                             SmoothingFunction smoothingType)
        : WrapperEffect(stage), fadeLength(fadeLength), smoothingFunction(smoothingType), startIndex(startIndex) {

}


void OpacityGradientEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->running == DONE)
        return;

    if (this->running == NOT_STARTED) {
        this->running = RUNNING;
    }


    this->stage->calculate(startIndex, tempData);

    for (int i = 0; i < abs(fadeLength); i++) {
        int sign = fadeLength < 0 ? -1 : 1;
        tempData.set(
                startIndex + i * sign + this->startIndex,
                tempData.get(startIndex + i * sign + this->startIndex),
                smoothingFunction(
                        0,
                        fadeLength,
                        0,
                        tempData.getOpacity(startIndex + i * sign + this->startIndex),
                        i
                )
        );
    }

    this->running = this->stage->running;
}