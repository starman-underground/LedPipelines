#include "enums/SmoothingFunction.h"
#include "math.h"

using namespace ledpipelines;


float SmoothingFunction::operator()(float amount, float oldMin, float oldMax, float newMin, float newMax) {
    float oldRange = (oldMax - oldMin);
    float amountInRange = (amount - oldMin);
    float percentage = amountInRange / oldRange;

    switch (this->value) {
        case SMOOTH_LINEAR:
            // f(x) = 0.5 * cos(pi * x + pi) + 0.5
            percentage = 0.5f * cosf(M_PI * percentage + M_PI) + 0.5f;
            break;
        case LINEAR:
            // f(x) = x, so we don't do anything.
            break;
        case SINE:
            // f(x) = sin(pi * x / 2)
            percentage = sinf(M_PI * percentage / 2);
            break;
    }

    return (newMin + (newMax - newMin) * percentage);
}
