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

RandomTimedEffect::RandomTimedEffect(float maxRuntime, SamplingFunction samplingFunction) :
        RandomTimedEffect(0, maxRuntime, samplingFunction) {}


void RandomTimedEffect::reset() {
    TimedEffect::reset();
}

void RandomTimedEffect::sampleRuntime() {
    this->timeToRunSeconds = samplingFunction(minRuntime, maxRuntime);
}

RandomTimedEffect::RandomTimedEffect(float minRuntime, float maxRuntime, SamplingFunction samplingFunction)
        : TimedEffect(0),
          minRuntime(0),
          maxRuntime(0),
          samplingFunction(samplingFunction) {

}

