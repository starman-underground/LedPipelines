#include "effects/TextEffect.h"

using namespace ledpipelines::effects;

TextEffect::TextEffect(const std::string &text, TwoDimensionalLayout layout, CRGB color, CRGB bg_color, uint8_t opacity)
    : BaseLedPipelineStage(BlendingMode::NORMAL),
    charWidth(FONT_WIDTH),
    charHeight(FONT_HEIGHT),
    text(text),
    layout(layout),
    bg_color(bg_color),
    color(color),
    opacity(opacity) {}

void TextEffect::calculate(float startX, TemporaryLedData &tempData) {
    // #TODO: This is a hack, we should allow the user to specify the display dimensions and text position/alignment.
    int displayHeight = 8;
    int displayWidth = 32;
    for (size_t c = 0; c < text.size(); ++c) {
        char currentChar = text[c];
        for (int i = 0; i < charHeight; ++i) {
            for (int j = 0; j < charWidth + 1; ++j) {
                int x  = static_cast<int>(startX) + c*(static_cast<int>(charWidth)+1) + j;
                int y = displayHeight - charHeight + i;
                int index = ledpipelines::calculateLedIndex(layout, x, y, displayWidth, displayHeight);
                if (index < TemporaryLedData::size && index >= 0 && x >= 0 && y >= 0 && x < displayWidth && y < displayHeight) {                    
                    if (ledpipelines::getPixel(currentChar, j, i)) {
                        tempData.set(index, color, opacity);
                    } else {
                        tempData.set(index, bg_color, 16); // Set to background color with reduced opacity
                    }
                }
            }
        }
    }

    // // Iterate through each pixel in the character's area
    // for (int i = 0; i < charHeight; ++i) {
    //     for (int j = 0; j < charWidth; ++j) {
    //         int index = ledpipelines::calculateLedIndex(layout, j, i, 32, 8);
    //         if (index < TemporaryLedData::size) {

    //             if (ledpipelines::getPixel(character, j, i)) {
    //                 // If the pixel is part of the character, set its color
    //                 tempData.set(index, color, opacity);
    //             } else {
    //                 // If the pixel is not part of the character, set it to black or transparent
    //                 tempData.set(index, CRGB::Black, 0); // Set to black with 0 opacity
    //             }
    //         }
    //     }
    // }
}