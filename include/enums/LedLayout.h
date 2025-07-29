#pragma once

#include <utility>

namespace ledpipelines {
class LedLayout {
public:
    int width, height;

    enum LedLayout_ {
        // Single wire layout, all LEDs in a single line
        ONE_DIMENSIONAL, 
        // LED wire snakes from top to bottom then bottom to top, starting from top left and going to the right
        VERTICAL_SNAKE,
        // LED wire snakes from top to bottom then bottom to top, starting from top right and going to the left
        VERTICAL_SNAKE_XFLIP,       
        // LED wire snakes from bottom to top then top to bottom, starting from bottom left and going to the right
        VERTICAL_SNAKE_YFLIP,
        // LED wire snakes from bottom to top then top to bottom, starting from bottom right and going to the left
        VERTICAL_SNAKE_XYFLIP,
        // LED wire snakes from left to right then right to left, starting from top left and going down
        HORIZONTAL_SNAKE,
        // LED wire snakes from right to left then left to right, starting from top right and going down
        HORIZONTAL_SNAKE_XFLIP,
        // LED wire snakes from left to right then right to left, starting from bottom left and going up
        HORIZONTAL_SNAKE_YFLIP,
        // LED wire snakes from right to left then left to right, starting from bottom right and going up
        HORIZONTAL_SNAKE_XYFLIP,
    };

    LedLayout(LedLayout_ layout, int width, int height) : value(layout), width(width), height(height) {}

    int calculateLedIndex(int x, int y);
    int calculateLedIndex(float x, float y) {
        return this->calculateLedIndex(static_cast<int>(x), static_cast<int>(y));
    }

    std::pair<float, float> calculateCoordinates(int ledIndex);
    std::pair<float, float> calculateCoordinates(float ledIndex) {
        // TODO: Add fractional support for 2D coordinates
        return this->calculateCoordinates(static_cast<int>(ledIndex));
    }

private:
    LedLayout_ value;
};


}