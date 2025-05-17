
#include "effects/MovingEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

MovingEffect::MovingEffect(BaseLedPipelineStage *stage, float ledsPerSecond, int startPosition, int endPosition)
        : WrapperEffect(stage),
          currentPosition(0),
          startPosition(startPosition),
          endPosition(endPosition),
          ledsPerSecond(ledsPerSecond),
          elapsedPercentage(0) {}

void MovingEffect::calculate(int startIndex, TemporaryLedData &tempData) {
    if (this->state == LedPipelineRunningState::DONE) return;

    if (this->state == LedPipelineRunningState::NOT_STARTED) {
        this->startTimeMillis = millis();
        this->state =  LedPipelineRunningState::RUNNING;
    }

    this->currentPosition = (ledsPerSecond * (millis() - this->startTimeMillis) / 1000.0f);

    if (ledsPerSecond < 0) {
        if (this->currentPosition <= this->endPosition) {
            this->state = this->stage->state;
            return;
        }
    } else {
        if (this->currentPosition >= this->endPosition) {
            this->state = this->stage->state;
            return;
        }
    }

    // calculate the offset due to the motion.
    int deltaLeds = (int) currentPosition;
    this->elapsedPercentage = this->currentPosition / this->endPosition;
    this->stage->calculate(startIndex + deltaLeds, tempData);
    if (this->stage->state == LedPipelineRunningState::DONE) {
        this->state = LedPipelineRunningState::DONE;
    }
}

void MovingEffect::reset() {
    WrapperEffect::reset();
    this->currentPosition = this->startPosition;
    this->elapsedPercentage = 0;
}
