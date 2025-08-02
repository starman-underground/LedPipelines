#pragma once

#include "SolidEffect.h"
#include "LedPipelineUtils.h"
#include <string>

namespace ledpipelines::effects {

class TextEffect : public BaseLedPipelineStage{

protected:
    LedLayout layout;
    std::string text;
    CRGB bg_color;
    CRGB color;
    uint8_t opacity;
    uint8_t bg_opacity;

public:
    explicit TextEffect(const std::string &text, LedLayout layout, CRGB color = CRGB::White, CRGB bg_color = CRGB::Black, uint8_t opacity = 255, uint8_t bg_opacity = 0)
        : BaseLedPipelineStage(BlendingMode::NORMAL),
          layout(layout), text(text), color(color), bg_color(bg_color), opacity(opacity), bg_opacity(bg_opacity)
    {};

    void calculate(float startIndex, TemporaryLedData &tempData) override;
};

}