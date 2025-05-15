#pragma once


#include "BaseEffect.h"

namespace ledpipelines::effects {
class PulsingEffect : public WrapperEffect, public TimedEffect {
public:
    PulsingEffect(BaseLedPipelineStage *stage, float time);
};

}