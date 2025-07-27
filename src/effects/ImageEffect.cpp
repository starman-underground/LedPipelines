#include "effects/ImageEffect.h"

using namespace ledpipelines::effects;

ImageEffect::ImageEffect(const uint8_t *imageData, TwoDimensionalLayout layout, int width, int height, uint8_t opacity)
    : BaseLedPipelineStage(BlendingMode::NORMAL),
      imageData(imageData),
      xPos(0.0f),
      yPos(0.0f),
      width(width),
      height(height),
      layout(layout) {}

void ImageEffect::calculate(float startX, TemporaryLedData &tempData) {
    // Assume the display dimensions are provided or known
    int displayWidth = 32; // Example width
    int displayHeight = 8; // Example height
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = ledpipelines::calculateLedIndex(layout, static_cast<int>(startX) + x, y, displayWidth, displayHeight);
            if (index >= 0 && index < TemporaryLedData::size) {
                int offset = (y * width + x) * 4;
                CRGB color(imageData[offset], imageData[offset + 1], imageData[offset + 2]);
                tempData.set(index, color, imageData[offset + 3]);
            }
        }
    }
}
    