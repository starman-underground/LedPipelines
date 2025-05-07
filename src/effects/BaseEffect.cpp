#include "effects/BaseEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

WrapperEffect::WrapperEffect(BaseLedPipelineStage *stage) :
        BaseLedPipelineStage(stage->blendingMode),
        stage(stage) {}

WrapperEffect::~WrapperEffect() {
    delete stage;
}

void WrapperEffect::reset() {
    BaseLedPipelineStage::reset();
    stage->reset();
}


TimedEffect::TimedEffect(float timeToRunSeconds) : timeToRunSeconds(timeToRunSeconds) {}

void TimedEffect::reset() {
    this->elapsedPercentage = 0;
}
