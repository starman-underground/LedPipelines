#include "effects/RepeatEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

RepeatEffect::RepeatEffect(BaseLedPipelineStage *stage, int repeatDistance, int numRepeats)
        : WrapperEffect(stage),
          numRepeats(numRepeats),
          repeatDistance(repeatDistance) {}


void RepeatEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->state == DONE)
        return;

    if (this->state == NOT_STARTED)
        this->state = RUNNING;

//    // only calculate the data once. We first calculate it at 0, and then shift it by how much
//    TemporaryLedData stageData = TemporaryLedData();
//    this->stage->calculate(0, stageData);
//
//    TemporaryLedData shiftedStageData = TemporaryLedData();
//
//    for (int i = 0; i < TemporaryLedData::size; i++) {
//        shiftedStageData.set(i + startIndex, stageData[i], stageData.opacity[i]);
//    }
//

    // shift it by x amount.
    if (numRepeats == 0) {
        // if num repeats isn't specified, then we use infinite repeats. Do this both forwards and backwards.
        int currentIndex = startIndex;
        bool currentStageSetsData = true;
        while (currentStageSetsData) {
            TemporaryLedData stageData = TemporaryLedData();
            this->stage->calculate(currentIndex, stageData);
            currentStageSetsData = stageData.anyAreModified || currentIndex < 0;
            tempData.merge(stageData, this->stage->blendingMode);
            currentIndex += repeatDistance;
        }
        currentStageSetsData = true;
        currentIndex = startIndex - repeatDistance;
        while (currentStageSetsData) {
            TemporaryLedData stageData = TemporaryLedData();
            this->stage->calculate(currentIndex, stageData);
            currentStageSetsData = stageData.anyAreModified;
            tempData.merge(stageData, this->stage->blendingMode);
            currentIndex -= repeatDistance;
        }
    } else {
        for (int i = 0; i < numRepeats; i++) {
            TemporaryLedData stageData = TemporaryLedData();
            this->stage->calculate(startIndex + i * repeatDistance, stageData);
            tempData.merge(stageData, this->stage->blendingMode);
        }
    }
    this->state = this->stage->state;
}
