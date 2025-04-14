#pragma once


#include "BaseEffect.h"

class PulsingEffect : public WrapperEffect, public TimedEffect {
public:
    PulsingEffect(BaseLedPipelineStage *stage, float time);
};
