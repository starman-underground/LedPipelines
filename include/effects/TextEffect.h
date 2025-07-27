#pragma once

#include "SolidEffect.h"
#include "LedPipelineUtils.h"
#include <string>

namespace ledpipelines::effects {

class TextEffect : public BaseLedPipelineStage{

protected:
    TwoDimensionalLayout layout;
    std::string text;
    CRGB bg_color;
    CRGB color;
    uint8_t opacity;

public:
    explicit TextEffect(const std::string &text, TwoDimensionalLayout layout, CRGB color = CRGB::Gold, CRGB bg_color = CRGB::Black, uint8_t opacity = 255);

    void calculate(float startX, TemporaryLedData &tempData) override;
};

}