#pragma once

#include "BaseLedPipeline.h"

namespace ledpipelines::effects {
    class SolidEffect : public BaseLedPipelineStage {
    protected:
        CRGB color;
        uint8_t opacity;

    public:
        explicit SolidEffect(CRGB color, uint8_t opacity = 255);

        void calculate(int startIndex, TemporaryLedData &tempData) override;
    };


    class SolidSegmentEffect : public SolidEffect {

    protected:
        int segmentLength = 0;

    public:
        explicit SolidSegmentEffect(CRGB color, int segmentLength, uint8_t opacity = 255);

        void calculate(int startIndex, TemporaryLedData &tempData) override;
    };

}