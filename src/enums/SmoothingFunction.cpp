#include "enums/SmoothingFunction.h"
#include <cmath>

using namespace ledpipelines;


float SmoothingFunction::operator()(float amount, float oldMin, float oldMax, float newMin, float newMax) {
    /**
     * min and max aren't guaranteed to actually be min and max. e.g. it could be min < amount < max, OR
     * max < amount < min. This logic clamps to the range (min, max) regardless of which one is smaller and which one
     * is larger.
     */
    if (oldMin < oldMax) {
        if (amount < oldMin) {
            amount = oldMin;
        } else if (amount > oldMax) {
            amount = oldMax;
        }
    } else {
        if (amount > oldMin) {
            amount = oldMin;
        } else if (amount < oldMax) {
            amount = oldMax;
        }

    }

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
