
#include <utility>

#include "effects/MovingEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

MovingEffect::MovingEffect(
        BaseLedPipelineStage *stage,
        float runtimeSeconds,
        float startPosition,
        float endPosition,
        SmoothingFunction smoothingFunction,
        LedLayout layout
        )
        : WrapperEffect(stage),
          startPosition(startPosition, 0),
          endPosition(endPosition, 0),
          runtimeSeconds(runtimeSeconds),
          elapsedPercentage(0),
          smoothingFunction(smoothingFunction),
          layout(layout) {}

MovingEffect::MovingEffect(
        BaseLedPipelineStage *stage,
        float runtimeSeconds,
        LedLayout layout,
        std::pair<float, float> startPosition,
        std::pair<float, float> endPosition,
        SmoothingFunction smoothingFunction
        )
        : WrapperEffect(stage),
          startPosition(startPosition),
          endPosition(endPosition),
          runtimeSeconds(runtimeSeconds),
          elapsedPercentage(0),
          smoothingFunction(smoothingFunction),
          layout(layout) {}

void MovingEffect::calculate(float startIndex, TemporaryLedData &tempData) {
    if (this->state == LedPipelineRunningState::DONE) return;
    this->calculate({startIndex, 0}, tempData);
}

void MovingEffect::calculate(std::pair<float, float> start, TemporaryLedData &tempData) {
    if (this->state == LedPipelineRunningState::DONE) return;

    if (this->state == LedPipelineRunningState::NOT_STARTED) {
        this->startTimeMillis = millis();
        this->state =  LedPipelineRunningState::RUNNING;
    }

    this->elapsedPercentage = (millis() - this->startTimeMillis) / runtimeSeconds / 1000.0f;

    auto currentPosition = smoothingFunction(elapsedPercentage, startPosition, endPosition);

    LPLogger::log(String("elapsed percentage: ") + elapsedPercentage + " current position: (" + currentPosition.first + ", " + currentPosition.second + ")");

    this->stage->calculate(layout.calculateLedIndex(start.first + currentPosition.first, start.second + currentPosition.second), tempData);

    if (this->stage->state == LedPipelineRunningState::DONE || this->elapsedPercentage >= 1) {
        this->state = LedPipelineRunningState::DONE;
    }
}

void MovingEffect::reset() {
    WrapperEffect::reset();
    this->elapsedPercentage = 0;
}
