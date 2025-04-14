
#pragma once

#include "BaseLedPipeline.h"

class WrapperEffect : virtual public BaseLedPipelineStage {
protected:
    BaseLedPipelineStage *stage;
public:
    explicit WrapperEffect(BaseLedPipelineStage *stage);

    ~WrapperEffect() override;

    void reset() override;
};





