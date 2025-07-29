#include "enums/LedLayout.h"

using namespace ledpipelines;

int LedLayout::calculateLedIndex(int x, int y) {
    switch (value) {
    case LedLayout::ONE_DIMENSIONAL:
        return x; // y is ignored in this layout
    case LedLayout::VERTICAL_SNAKE:
        return (x % 2 == 0) ? (x*height + y) : (x*height + (height - 1 - y));
    case LedLayout::VERTICAL_SNAKE_XFLIP:
        return ((width - 1 - x) % 2 == 0) ? ((width - 1 - x)*height + y) : ((width - 1 - x)*height + (height - 1 - y));
    case LedLayout::VERTICAL_SNAKE_YFLIP:
        return (x % 2 == 0) ? (x*height + (height - 1 - y)) : (x*height + y);
    case LedLayout::VERTICAL_SNAKE_XYFLIP:
        return ((width - 1 - x) % 2 == 0) ? ((width - 1 - x)*height + (height - 1 - y)) : ((width - 1 - x)*height + y);
    case LedLayout::HORIZONTAL_SNAKE:
        return (y % 2 == 0) ? (x + y*width) : ((width - 1 - x) + y*width);
    case LedLayout::HORIZONTAL_SNAKE_XFLIP:
        return (y % 2 == 0) ? ((width - 1 - x) + y*width) : (x + y*width);
    case LedLayout::HORIZONTAL_SNAKE_YFLIP:
        return ((height - 1 - y) % 2 == 0) ? (x + (height - 1 - y)*width) : ((width - 1 - x) + (height - 1 - y)*width);
    case LedLayout::HORIZONTAL_SNAKE_XYFLIP:
        return ((height - 1 - y) % 2 == 0) ? ((width - 1 - x) + (height - 1 - y)*width) : (x + (height - 1 - y)*width);
    }
}

std::pair<float, float> LedLayout::calculateCoordinates(int ledIndex) {
    switch (value) {
    case LedLayout::ONE_DIMENSIONAL:
        return { static_cast<float>(ledIndex), 0.0f };
    case LedLayout::VERTICAL_SNAKE:
        return { (ledIndex / height), (ledIndex / height) % 2 == 0 ? (ledIndex % height) : (height - 1 - (ledIndex % height)) };
    case LedLayout::VERTICAL_SNAKE_XFLIP:
        return { width - 1 - (ledIndex / height), (ledIndex / height) % 2 == 0 ? (ledIndex % height) : (height - 1 - (ledIndex % height)) };
    case LedLayout::VERTICAL_SNAKE_YFLIP:
        return { (ledIndex / height), (ledIndex / height) % 2 == 1 ? (ledIndex % height) : (height - 1 - (ledIndex % height)) };
    case LedLayout::VERTICAL_SNAKE_XYFLIP:
        return { width - 1 - (ledIndex / height), (ledIndex / height) % 2 == 1 ? (ledIndex % height) : (height - 1 - (ledIndex % height)) };
    case LedLayout::HORIZONTAL_SNAKE:
        return { (ledIndex / width) % 2 == 0 ? (ledIndex % width) : (width - 1 - (ledIndex % width)), (ledIndex / width) };
    case LedLayout::HORIZONTAL_SNAKE_XFLIP:
        return { (ledIndex / width) % 2 == 1 ? (ledIndex % width) : (width - 1 - (ledIndex % width)), (ledIndex / width) };
    case LedLayout::HORIZONTAL_SNAKE_YFLIP:
        return { (ledIndex / width) % 2 == 0 ? (ledIndex % width) : (width - 1 - (ledIndex % width)), height - 1 - (ledIndex / width) };
    case LedLayout::HORIZONTAL_SNAKE_XYFLIP:
        return { (ledIndex / width) % 2 == 1 ? (ledIndex % width) : (width - 1 - (ledIndex % width)), height - 1 - (ledIndex / width) };
    }
}