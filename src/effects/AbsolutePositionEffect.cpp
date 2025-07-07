#include "effects/AbsolutePositionEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

AbsolutePositionEffect::AbsolutePositionEffect(BaseLedPipelineStage *stage, float startPosition)
        : WrapperEffect(stage), startPosition(startPosition) {}

void AbsolutePositionEffect::calculate(float startIndex, TemporaryLedData &tempData) {
    this->stage->calculate(startPosition, tempData);
    this->state = this->stage->state;
}


