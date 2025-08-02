#pragma once

#include <utility>


namespace ledpipelines {
class SmoothingFunction {

public:

    enum SmoothingFunction_ {
        SMOOTH_LINEAR,
        LINEAR,
        SINE,
        QUADRATIC,
        INVERSE_QUADRATIC
    };

    SmoothingFunction(const SmoothingFunction_ &function) : value(function) {}

    SmoothingFunction(const SmoothingFunction_ &&function) : value(function) {}

    SmoothingFunction(const SmoothingFunction &function) : value(function.value) {}

    SmoothingFunction(const SmoothingFunction &&function) : value(function.value) {}


    float operator()(float percentage) {
        return this->operator()(percentage, 0, 1);
    }

    float operator()(float percentage, float max) {
        return this->operator()(percentage, 0, max);
    }

    float operator()(float percentage, float min, float max) {
        return this->operator()(percentage, 0, 1, min, max);
    }

    float operator()(float amount, float oldMin, float oldMax, float newMin, float newMax);

    std::pair<float, float> operator()(float percentage, std::pair<float, float> max) {
        return this->operator()(percentage, std::pair<float, float>(0.0f, 0.0f), max);
    }

    std::pair<float, float> operator()(float percentage, std::pair<float, float> min, std::pair<float, float> max) {
        return this->operator()(percentage, {0, 0}, {1, 1}, min, max);
    }

    std::pair<float, float> operator()(float percentage,
                     std::pair<float, float> oldMin,
                     std::pair<float, float> oldMax,
                     std::pair<float, float> newMin,
                     std::pair<float, float> newMax);

private:
    SmoothingFunction_ value;

};

}
