#pragma once

#include <FastLED.h>

CRGB operator*(CRGB first, CRGB second);

CRGB &operator*=(CRGB &first, const CRGB &second);

enum BlendingMode {
    OVERWRITE,
    ADD,
    MULTIPLY
};

/**
 * A class representing a buffer of data used to build out effects. Each effect populates
 * a buffer passed to it in its calculate() method. Buffers are meant to be short lived.
 * They hold enough information for every LED in FastLED, including multiple strips.
 */
class TemporaryLedData {

private:
public:
    /**
     * The internal data stored in the temporary buffer. This is heap memory created when the buffer
     * is instantiated, and deleted afterwards.
     */
    CRGB *data;

    /**
     *The total number of LEDs added to FastLED. This is populated in the initialize() method.
     */
    static int numLeds;

    /**
     * Initialize static information needed for all LED data. This should be called
     * before any calls to pipelines (run, calculate, or anything else.).
     */
    static void initialize ();


    /**
     * an operator to make accessing data as simple as it is for CRGB structs. It can be populated
     * directly with CRGB data, the same as a normal CRGB array.d
     * @param index the index in the temporary data to access / write to.
     * @return the CRGB data living at that index in the temporary buffer.
     */
    CRGB &operator[](int index) {
        return data[index];
    }


    TemporaryLedData();

    ~TemporaryLedData();

    void merge(TemporaryLedData &other, BlendingMode blendingMode);

    void populateFastLed();
};

class BaseLedPipelineStage {
public:

    bool running = true;

    BlendingMode blendingMode = ADD;

    BaseLedPipelineStage *nextStage = nullptr;

    virtual void calculate(int startIndex, TemporaryLedData tempData) = 0;

    virtual void reset();

    explicit BaseLedPipelineStage(BlendingMode blendingMode = ADD);

    virtual ~BaseLedPipelineStage();
};


/**
 * Base abstract class for defining LED pipelines - the construct that can be run
 * and show effects on the
 */
class LedPipeline : public BaseLedPipelineStage {

public:

    virtual LedPipeline *addStage(BaseLedPipelineStage *stage);

    virtual void run();

    void reset() override;

    virtual ~LedPipeline();

protected:
    /**
     * linked list of first and last stages. used when adding stages and running them.
     */
    BaseLedPipelineStage *firstStage = nullptr;
    BaseLedPipelineStage *lastStage = nullptr;

};


/**
 * A pipeline that runs all its stages in parallel. The stage that gets added first
 * gets applied first, with other layers getting applied on top. The pipeline continues
 * until all stages have completed.
 */
class ParallelLedPipeline : public LedPipeline {

public:

    void calculate(int startIndex, TemporaryLedData tempData) override;

};

/**
 * A pipeline that blocks every stage before it moves on to the next one. The stage must be considered complete (the
 * calculate function must return false) before it moves on to the next stage. This is useful for functions that need
 * to happen one after another, not at the same time.
 */
class SeriesLedPipeline : public LedPipeline {

public:
    void calculate(int startIndex, TemporaryLedData tempData) override;

    void reset() override;

private:
    BaseLedPipelineStage *currentStage = nullptr;
};



