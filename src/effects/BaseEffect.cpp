#include "effects/BaseEffect.h"


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


TimedEffect::TimedEffect(long timeToRunMillis) : timeToRunMillis(timeToRunMillis) {}

void TimedEffect::reset() {
    this->elapsedPercentage = 0;
}
