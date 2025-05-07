#include "effects/MaskEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

MaskEffect::MaskEffect(
        BaseLedPipelineStage *base,
        BaseLedPipelineStage *mask,
        bool startFullOpacity
) : base(base),
    mask(mask),
    startFullOpacity(startFullOpacity) {
}


void MaskEffect::calculate(int startIndex, TemporaryLedData &tempData) {

    if (this->running == DONE) {
        return;
    }

    if (this->running == NOT_STARTED) {
        this->running = RUNNING;
    }

    TemporaryLedData maskData = TemporaryLedData();
    TemporaryLedData baseData = TemporaryLedData();
    for (int i = 0; i < TemporaryLedData::size; i++) {
        maskData.opacity[i] = this->startFullOpacity ? 255 : 0;
    }
    this->mask->calculate(startIndex, maskData);
    this->base->calculate(startIndex, baseData);

    // merge the two layers.
    baseData.merge(maskData, MASK);
    tempData.merge(baseData, this->base->blendingMode);

    // The effect finishes when the base is done, so we check if the base effect is done here.
    if (this->base->running == DONE) {
        this->running = DONE;
    }
}


void MaskEffect::reset() {
    BaseLedPipelineStage::reset();
    base->reset();
    mask->reset();
}
