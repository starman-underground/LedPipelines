#pragma once

#include "FastLED.h"


enum LedPipelinesLogLevel {
    NONE,
    ERROR,
    WARN,
    LOG,
    VERBOSE
};


enum LedPipelinesSmoothingType {
    LINEAR,
    SINE,
    CUBIC,

};

struct LedPipelinesUtils {
public:
    static LedPipelinesLogLevel logLevel;

    static void setLogLevel(LedPipelinesLogLevel logLevel) { LedPipelinesUtils::logLevel = logLevel; }

    static void log(LedPipelinesLogLevel logLevel, String log);
};


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
public:
    /**
     * The internal data stored in the temporary buffer. This is heap memory created when the buffer
     * is instantiated, and deleted afterwards.
     */
    CRGB *data;

    /**
     * To tell in the merging algorithm whether to merge the current pixel, we need to know if it was modified
     * at all. We can do this using another array that stores the modified value.
     */
    bool *modified;

    /**
     *The total number of LEDs added to FastLED. This is populated in the initialize() method.
     */
    static int size;

    /**
     * Initialize static information needed for all LED data. This should be called
     * before any calls to pipelines (run, calculate, or anything else.).
     */
    static void initialize();


    /**
     * an operator to make accessing data as simple as it is for CRGB structs. It can be populated
     * directly with CRGB data, the same as a normal CRGB array.d
     * @param index the index in the temporary data to access / write to.
     * @return the CRGB data living at that index in the temporary buffer.
     */
    CRGB &operator[](int index) const {
        return data[index];
    }

    TemporaryLedData();

    ~TemporaryLedData();

    void merge(TemporaryLedData &other, BlendingMode blendingMode);

    void populateFastLed() const;

    void printData() const;

    void set(int index, CRGB &color);
};

