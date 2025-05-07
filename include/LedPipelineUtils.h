#pragma once

#include "FastLED.h"


CRGB operator*(CRGB first, CRGB second);

CRGB &operator*=(CRGB &first, const CRGB &second);

namespace ledpipelines {

    enum LedPipelinesLogLevel {
        NONE,
        ERROR,
        WARN,
        LOG,
        Debug
    };


    enum LedPipelinesSmoothingType {
        LINEAR,
        SINE,
        SMOOTH_LINEAR
    };


    float interpolate(LedPipelinesSmoothingType type, float oldStart, float oldEnd, float newStart, float newEnd,
                      float amount);

    float interpolate(LedPipelinesSmoothingType type, float newStart, float newEnd, float amount);


    struct LPLogger {

    private:
        static void logInternal(LedPipelinesLogLevel logLevel, String &log);

    public:
        static LedPipelinesLogLevel logLevel;

        static void setLogLevel(LedPipelinesLogLevel logLevel) { LPLogger::logLevel = logLevel; }

        static void log(String log) { LPLogger::logInternal(LOG, log); }

        static void log(int log) { LPLogger::log(String(log)); }

        static void log(unsigned int log) { LPLogger::log(String(log)); }

        static void log(long log) { LPLogger::log(String(log)); }

        static void log(unsigned long log) { LPLogger::log(String(log)); }

        static void log(char log) { LPLogger::log(String(log)); }

        static void log(float log) { LPLogger::log(String(log)); }

        static void log(double log) { LPLogger::log(String(log)); }

        static void warn(String warn) { LPLogger::logInternal(WARN, warn); }

        static void warn(int warn) { LPLogger::warn(String(warn)); }

        static void warn(unsigned int warn) { LPLogger::warn(String(warn)); }

        static void warn(long warn) { LPLogger::warn(String(warn)); }

        static void warn(unsigned long warn) { LPLogger::warn(String(warn)); }

        static void warn(char warn) { LPLogger::warn(String(warn)); }

        static void warn(float warn) { LPLogger::warn(String(warn)); }

        static void warn(double warn) { LPLogger::warn(String(warn)); }

        static void error(String error) { LPLogger::logInternal(ERROR, error); }

        static void error(int error) { LPLogger::error(String(error)); }

        static void error(unsigned int error) { LPLogger::error(String(error)); }

        static void error(long error) { LPLogger::error(String(error)); }

        static void error(unsigned long error) { LPLogger::error(String(error)); }

        static void error(char error) { LPLogger::error(String(error)); }

        static void error(float error) { LPLogger::error(String(error)); }

        static void error(double error) { LPLogger::error(String(error)); }

        static void debug(String log) { LPLogger::logInternal(Debug, log); }

        static void debug(int log) { LPLogger::debug(String(log)); };

        static void debug(unsigned int log) { LPLogger::debug(String(log)); };

        static void debug(long log) { LPLogger::debug(String(log)); };

        static void debug(unsigned long log) { LPLogger::debug(String(log)); };

        static void debug(char log) { LPLogger::debug(String(log)); };

        static void debug(float log) { LPLogger::debug(String(log)); };

        static void debug(double log) { LPLogger::debug(String(log)); };
    };


    enum BlendingMode {
        NORMAL,
        OVERWRITE,
        ADD,
        MULTIPLY,
        MASK,
    };


/**
 * Convert a Color and opacity to a hex code, in form FFFFFFFF, or RGBA.
 * @param color the color to convert, in RGB form.
 * @param opacity the opacity of the color to convert.
 * @return the color in Hex form in a string.
 */
    String colorToHex(CRGB color, uint8_t opacity);


/**
 * A class representing a buffer of data used to build out effects. Each effect populates
 * a buffer passed to it in its calculate() method. Buffers are meant to be short lived.
 * They hold enough information for every LED in FastLED, including multiple strips.
 */
    class TemporaryLedData {

    private:
        /**
         * Store the start indices so they can be recalled when setting in 2D matrix form (e.g. setting pixels on specific strips).
         */
        static int *startIndexes;


    public:
        /**
         * The internal data stored in the temporary buffer. This is heap memory created when the buffer
         * is instantiated, and deleted afterwards.
         */
        CRGB *data;

        /**
         * To tell in the merging algorithm whether to merge the current pixel, we need to know if it was used
         * at all. We can do this using another array that stores the opacity value.
         */
        uint8_t *opacity;

        /**
         * Tracks if any LEDs have been set. Default to false, changes to true when a call to set() results in an LED
         * being set.
         */
        bool anyAreModified = false;

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
         * directly with CRGB data, the same as a normal CRGB array.
         * @param index the index in the temporary data to access / write to.
         * @return the CRGB data living at that index in the temporary buffer.
         */
        CRGB &operator[](int index) const {
            return data[index];
        }

        /**
         * Constructor for creating LED data. Will create a buffer dynamically based on how many LEDs are added in
         */
        TemporaryLedData(CRGB color = CRGB::Black);

        ~TemporaryLedData();

        void merge(TemporaryLedData &other, BlendingMode blendingMode);

        void populateFastLed() const;

        void printData() const;

        void set(int index, CRGB color, uint8_t opacity = UINT8_MAX);

        void set(int stripIndex, int ledIndex, CRGB &color, uint8_t opacity = UINT8_MAX);

        CRGB get(int index) const;

        uint8_t getOpacity(int index) const;
    };

}