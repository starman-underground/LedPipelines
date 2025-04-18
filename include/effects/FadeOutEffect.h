#pragma once


#include "BaseEffect.h"

class FadeOutEffect : public WrapperEffect, TimedEffect {


public:
    float startOffsetSeconds;
    float fadeTime;


};