#pragma once


namespace ledpipelines {
class SmoothingFunction {

public:

    enum SmoothingFunction_ {
        SMOOTH_LINEAR,
        LINEAR,
        SINE,
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


private:
    SmoothingFunction_ value;

};

}
