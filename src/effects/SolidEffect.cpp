#include "effects/SolidEffect.h"

using namespace ledpipelines;
using namespace ledpipelines::effects;

SolidEffect::SolidEffect(CRGB color, uint8_t opacity) :
        BaseLedPipelineStage(BlendingMode::NORMAL),
        color(color),
        opacity(opacity) {
    this->state = LedPipelineRunningState::RUNNING;
}

void SolidEffect::calculate(float startIndex, TemporaryLedData &tempData) {
    for (int i = startIndex; i < TemporaryLedData::size; i++) {
        tempData.set(i, color, opacity);
    }
}


SolidSegmentEffect::SolidSegmentEffect(CRGB color, float segmentLength, uint8_t opacity)
        : SolidEffect(color, opacity),
          segmentLength(segmentLength) {}

void SolidSegmentEffect::calculate(float startIndex, TemporaryLedData &tempData) {


    float endIndex = startIndex + segmentLength;

    int startIndexFloor = (int) startIndex;
    int endIndexFloor = (int) endIndex;

    /**
     * Two cases: if we are trying to create a segment that is less than one pixel.
     *
     * if the start and end of the segment are in the same pixel, then we need to only light up that pixel, and only
     * partially based on how large the pixel value is.
     *
     * if the start is on a different pixel than the end, we can light up the first pixel partially, then light up
     * all pixels between start and end completely, and then light up the last pixel partially as well.
     */


    if (startIndexFloor == endIndexFloor) {
        // both are on the same pixel, we can light it up partially.
        uint8_t amountToLightUp = segmentLength * 255;
        tempData.set(startIndexFloor, color, amountToLightUp);

    } else {

        uint8_t amountToLightUpFirstPixel = (1 - (startIndex - startIndexFloor)) * 255;
        uint8_t amountToLightUpLastPixel = (endIndex - endIndexFloor) * 255;

        tempData.set(startIndexFloor, color, amountToLightUpFirstPixel);
        for (int i = startIndexFloor + 1; i < endIndexFloor ; i++) {
            tempData.set(i, color);
        }
        tempData.set(endIndexFloor, color, amountToLightUpLastPixel);

    }
}