#pragma once


#include "BaseEffect.h"

class OffsetEffect : public WrapperEffect {

public:
    int offset;

public:
     OffsetEffect(BaseLedPipelineStage *stage, int offset);

     void calculate(int startIndex, TemporaryLedData &tempData) override;
};