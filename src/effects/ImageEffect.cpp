#include "effects/ImageEffect.h"

using namespace ledpipelines::effects;

ImageEffect::ImageEffect(const uint8_t *imageData, LedLayout layout, int width, int height, uint8_t opacity)
    : BaseLedPipelineStage(BlendingMode::NORMAL),
      imageData(imageData),
      xPos(0.0f),
      yPos(0.0f),
      width(width),
      height(height),
      layout(layout),
      opacity(opacity) {}

void ImageEffect::calculate(std::pair<float, float> startPos, TemporaryLedData &tempData) {
    int startX = static_cast<int>(startPos.first);
    int startY = static_cast<int>(startPos.second);
    for (int iy = 0; iy < height; ++iy) {
        for (int ix = 0; ix < width; ++ix) {
            int x = startX + ix;
            int y = startY + iy;
            if (x < 0 || x >= layout.width || y < 0 || y >= layout.height) {
                continue; // Skip out-of-bounds pixels
            }
            int index = layout.calculateLedIndex(x, y);
            if (index >= 0 && index < TemporaryLedData::size) {
                int offset = (iy * width + ix) * 4;
                CRGB color(imageData[offset], imageData[offset + 1], imageData[offset + 2]);
                tempData.set(index, color, static_cast<uint8_t>((static_cast<float>(imageData[offset + 3])/255.0f)*opacity));
            }
        }
    }
}
    