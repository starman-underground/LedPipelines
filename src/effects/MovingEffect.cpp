
#include <utility>

#include "effects/MovingEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

MovingEffect::MovingEffect(
        BaseLedPipelineStage *stage,
        float runtimeSeconds,
        float startPosition,
        float endPosition,
        SmoothingFunction smoothingFunction
        )
        : WrapperEffect(stage),
          currentPosition(0),
          startPosition(startPosition),
          endPosition(endPosition),
          runtimeSeconds(runtimeSeconds),
          elapsedPercentage(0),
          smoothingFunction(smoothingFunction) {}

void MovingEffect::calculate(float startIndex, TemporaryLedData &tempData) {
    if (this->state == LedPipelineRunningState::DONE) return;

    if (this->state == LedPipelineRunningState::NOT_STARTED) {
        this->startTimeMillis = millis();
        this->state =  LedPipelineRunningState::RUNNING;
    }

    this->elapsedPercentage = (millis() - this->startTimeMillis) / runtimeSeconds / 1000.0f;


    this->currentPosition = smoothingFunction(elapsedPercentage, startPosition, endPosition);

    LPLogger::log(String("elapsed percentage: ") + elapsedPercentage + " current position: " + currentPosition);

    this->stage->calculate(startIndex + currentPosition, tempData);

    if (this->stage->state == LedPipelineRunningState::DONE || this->elapsedPercentage >= 1) {
        this->state = LedPipelineRunningState::DONE;
    }
}

void MovingEffect::reset() {
    WrapperEffect::reset();
    this->currentPosition = this->startPosition;
    this->elapsedPercentage = 0;
}
