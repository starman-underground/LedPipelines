#include "effects/MaskEffect.h"


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
    for (int i = 0; i < TemporaryLedData::size; i++) {
        maskData.opacity[i] = this->startFullOpacity ? 255 : 0;
    }
    this->mask->calculate(startIndex, maskData);
    this->base->calculate(startIndex, tempData);

    // The effect finishes when the base is done, so we check if the base effect is done here.
    if (this->base->running == DONE) {
        this->running = DONE;
    }

    // merge the two layers.
    tempData.merge(maskData, MASK);
}


void MaskEffect::reset() {
    BaseLedPipelineStage::reset();
    base->reset();
    mask->reset();
}
