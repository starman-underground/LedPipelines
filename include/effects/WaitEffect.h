#pragma once



#include "BaseEffect.h"

class WaitEffect : BaseLedPipelineStage {
public:
    /**
     * The amount of time already waited as a percentage of waitTimeSeconds. To get the actual amount of time waited,
     * do
     */
    float timeWaitingPercentage;

    /**
     * The time to run this effect, in seconds.
     */
    float waitTimeSeconds;

    /**
     * The time waiting started, in ms.
     */
    unsigned long startTimeMs;


    WaitEffect(float waitTimeSeconds);

    void calculate(int startIndex, TemporaryLedData &tempData) override;

    void reset() override;

};