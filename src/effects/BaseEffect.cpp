#include "effects/BaseEffect.h"


WrapperEffect::WrapperEffect(BaseLedPipelineStage *stage) :
        stage(stage) {}

WrapperEffect::~WrapperEffect() {
    delete stage;
}

void WrapperEffect::reset() {
    BaseLedPipelineStage::reset();
    stage->reset();
}




