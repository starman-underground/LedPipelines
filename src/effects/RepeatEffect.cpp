#include "effects/RepeatEffect.h"

RepeatEffect::RepeatEffect(BaseLedPipelineStage *stage, int numRepeats, int repeatDistance)
        : WrapperEffect(stage),
          numRepeats(numRepeats),
          repeatDistance(repeatDistance) {}

void RepeatEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (numRepeats <= 0) {

    }

}
