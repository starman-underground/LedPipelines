#include "effects/TextEffect.h"

using namespace ledpipelines::effects;

void TextEffect::calculate(float startIndex, TemporaryLedData &tempData) {
    int currentAdvance = 0;
    auto [startX, startY] = layout.calculateCoordinates(static_cast<int>(startIndex));
    for (size_t c = 0; c < text.size(); ++c) {
        char currentChar = text[c];
        GlyphMetadata gm = getGlyphMetadata(currentChar);
        for (int i = 0; i < gm.height; ++i) {
            for (int j = 0; j < gm.width; ++j) {
                int x  = static_cast<int>(startX) + currentAdvance + gm.xOffset + j;
                int y = 1 + static_cast<int>(startY) + gm.yOffset + i;
                int index = layout.calculateLedIndex(x, y);
                if (index < TemporaryLedData::size && index >= 0) {                    
                    if (getGlyphPixel(currentChar, j, i)) {
                        tempData.set(index, color, opacity);
                    } else {
                        tempData.set(index, bg_color, bg_opacity);
                    }
                }
            }
        }
        currentAdvance += gm.advanceWidth;
    }
}