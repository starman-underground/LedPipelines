#include "effects/AbsolutePositionEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

AbsolutePositionEffect::AbsolutePositionEffect(BaseLedPipelineStage *stage, int startPosition)
        : WrapperEffect(stage), startPosition(startPosition) {}

void AbsolutePositionEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    this->stage->calculate(startPosition, tempData);
    this->running = this->stage->running;
}


