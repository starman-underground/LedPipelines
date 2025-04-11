
#pragma once

#include "BaseLedPipeline.h"

class WrapperEffect : public BaseLedPipelineStage {
protected:
    BaseLedPipelineStage *stage;
public:
    explicit WrapperEffect(BaseLedPipelineStage *stage);

    ~WrapperEffect() override;

    void reset() override;
};





