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
    TwoDimensionalLayout layout;

public:
    explicit ImageEffect(const uint8_t *imageData, TwoDimensionalLayout layout, int width, int height, uint8_t opacity = 255);
    void calculate(float startX, TemporaryLedData &tempData) override;
};

}