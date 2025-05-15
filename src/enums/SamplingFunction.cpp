#include "enums/SamplingFunction.h"

using namespace ledpipelines;

float SamplingFunction::operator()(float min, float max) {
    float originalRandomValue = random();

    float randomFunctionOutput;

    switch (value) {
        case UNIFORM:
            randomFunctionOutput = originalRandomValue;
            break;
        case CENTERED:
            randomFunctionOutput = tanh(5 * (originalRandomValue - 0.5)) / tanh_range;
            break;
    }

    return (max - min) * randomFunctionOutput + min;
}
