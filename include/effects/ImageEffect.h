#pragma once

#include "BaseLedPipeline.h"
#include "LedPipelineUtils.h"
#include <string>

namespace ledpipelines::effects {

class ImageEffect : public BaseLedPipelineStage{

protected:
    const uint8_t *imageData;
    float xPos;
    float yPos;
    int width;
    int height;
    uint8_t opacity;
    LedLayout layout;

public:
    explicit ImageEffect(const uint8_t *imageData, LedLayout layout, int width, int height, uint8_t opacity = 255);
    void calculate(float startIndex, TemporaryLedData &tempData) override {
        calculate(layout.calculateCoordinates(startIndex), tempData);
    }
    void calculate(std::pair<float, float>, TemporaryLedData &tempData);
};

}