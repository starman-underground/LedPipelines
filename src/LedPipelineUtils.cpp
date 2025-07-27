
#include "LedPipelineUtils.h"

using namespace ledpipelines;

CRGB operator*(const CRGB first, const CRGB second) {
    return CRGB(
            (first.r * second.r) / 255,
            (first.g * second.g) / 255,
            (first.b * second.b) / 255
    );
}

CRGB &operator*=(CRGB &first, const CRGB &second) {
    first.r = (first.r * second.r) / 255;
    first.g = (first.g * second.g) / 255;
    first.b = (first.b * second.b) / 255;
    return first;
}

CRGB operator*(const CRGB first, const float amount) {
    return CRGB(
            first.r * amount,
            first.g * amount,
            first.b * amount
    );
}

CRGB &operator*=(CRGB &first, const float amount) {
    first.r = first.r * amount;
    first.g = first.g * amount;
    first.b = first.b * amount;
    return first;
}

uint64_t ledpipelines::minMicrosBetweenUpdates = 0;

void ledpipelines::setMaxRefreshRate(float refreshesPerSecond) {
    ledpipelines::minMicrosBetweenUpdates = (long) (1000000 / refreshesPerSecond);
}

static char HexLookUp[] = "0123456789abcdef";

static String byteToHex(uint8_t num) {
    uint8_t firstDigit = (num / 0x10);
    uint8_t secondDigit = (num % 0x10);
    return String(HexLookUp[firstDigit]) + HexLookUp[secondDigit];
}

String ledpipelines::colorToHex(CRGB color, uint8_t opacity) {
    return String(byteToHex(color.r) + byteToHex(color.g) + byteToHex(color.b) + byteToHex(opacity));
}

bool ledpipelines::getPixel(char c, int x, int y) {
    if (c < FIRST_ASCII || c > LAST_ASCII) return false;
    if (x < 0 || x >= FONT_WIDTH || y < 0 || y >= FONT_HEIGHT) return false;

    int charIndex = c - FIRST_ASCII;
    int pixelindex = y*FONT_WIDTH + x;
    int bitIndex = charIndex*BITS_PER_CHARACTER + pixelindex;
    int byteIndex = bitIndex >> 3;
    int bitOffset = 7 - (bitIndex & 7);

    uint8_t byte = pgm_read_byte(&COMPRESSED_FONT_DATA[byteIndex]);
    return (byte >> bitOffset) & 1;
}

String printCharacter(char c) {
    String result;
    if (c < FIRST_ASCII || c > LAST_ASCII) {
        result = "Invalid character";
    } else {
        for (int y = 0; y < FONT_HEIGHT; ++y) {
            for (int x = 0; x < FONT_WIDTH; ++x) {
                result += getPixel(c, x, y) ? '#' : '.';
            }
            result += '\n';
        }
    }
    return result;
}

int ledpipelines::calculateLedIndex(TwoDimensionalLayout layout, int x, int y, int w, int h) {
    switch (layout) {
        case TwoDimensionalLayout::VERTICAL_SNAKE: // left to right, top to bottom then bottom to top
            return (x % 2 == 0) ? ((x - 1)*h + y) : ((x - 1)*h + (h - 1 - y));
        case TwoDimensionalLayout::VERTICAL_SNAKE_XFLIP: // right to left, top to bottom then bottom to top
            return (x % 2 == 0) ? ((w - 1 - x)*h + (h - 1 - y)) : ((w - 1 - x)*h + y);
        case TwoDimensionalLayout::VERTICAL_SNAKE_YFLIP: // left to right, bottom to top then top to bottom
            return (x % 2 == 0) ? ((x - 1)*h + (h - 1 - y)) : ((x - 1)*h + y);
        case TwoDimensionalLayout::VERTICAL_SNAKE_XYFLIP: // right to left, bottom to top then top to bottom
            return (x % 2 == 0) ? ((w - 1 - x)*h + y) : ((w - 1 - x)*h + (h - 1 - y));
        case TwoDimensionalLayout::HORIZONTAL_SNAKE: // top to bottom, left to right then right to left
            return (y % 2 == 0) ? (x + y*w) : ((w - 1 - x) + y*w);
        case TwoDimensionalLayout::HORIZONTAL_SNAKE_XFLIP: // top to bottom, right to left then left to right
            return (y % 2 == 0) ? ((w - 1 - x) + y*w) : (x + y*w);
        case TwoDimensionalLayout::HORIZONTAL_SNAKE_YFLIP: // bottom to top, left to right then right to left
            return (y % 2 == 0) ? (x + (h - 1 - y)*w) : ((w - 1 - x) + (h - 1 - y)*w);
        case TwoDimensionalLayout::HORIZONTAL_SNAKE_XYFLIP: // bottom to top, right to left then left to right
            return (y % 2 == 0) ? ((w - 1 - x) + (h - 1 - y)*w) : (x + (h - 1 - y)*w);
    }
}