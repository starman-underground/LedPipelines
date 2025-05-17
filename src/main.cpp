#include "FastLED.h"
#include "LedPipelines.h"


using namespace ledpipelines;
using namespace ledpipelines::effects;


#define HEADER_1_PIN 2
#define HEADER_2_PIN 40
#define HEADER_3_PIN 37
#define HEADER_4_PIN 45
#define HEADER_5_PIN 47

#define HEADER_LED_STRIP_LENGTH 14
#define EXHAUST_LED_STRIP_LENGTH 24

#define BASE_COLOR  CRGB::DarkRed
#define PULSE_COLOR 0xFF2000
#define STRIP_TIME_OFFSET 4

BaseLedPipelineStage *pipeline;

CRGB leds[HEADER_LED_STRIP_LENGTH * 4 + EXHAUST_LED_STRIP_LENGTH];

/**
 * Creates a new effect that limits the specific effect to a specified strip. If the strip number is 0-3, the strip
 * will be shown on a specific header. Otherwise (strip = 4), the strip will be shown on the exhaust.
 * @param stage the effect that will be limited to the specific strip
 * @param strip the strip number to limit the effect to. 0-3 = headers, 4 = exhaust.
 * @return an OffsetEffect that can be used in other effects.
 */
BaseLedPipelineStage *limitEffectToStrip(BaseLedPipelineStage *stage, int strip) {
    auto length = (strip <= 3) ? HEADER_LED_STRIP_LENGTH : EXHAUST_LED_STRIP_LENGTH;

    float firing_order[] = {0, 2, 3, 1, 0};

    return (new SeriesLedPipeline())
            ->addStage(new WaitEffect(STRIP_TIME_OFFSET * firing_order[strip]))
            ->addStage(
                    new OffsetEffect(
                            new MaskEffect(
                                    stage,
                                    new SolidSegmentEffect(CRGB::White, length),
                                    false
                            ),
                            HEADER_LED_STRIP_LENGTH * strip
                    )
            );
}


BaseLedPipelineStage *pulseEffect(
        float minWaitTime,
        float maxWaitTime,
        float minFadeInTime,
        float maxFadeInTime,
        float minFullEffectTime,
        float maxFullEffectTime,
        float minFadeOutTime,
        float maxFadeOutTime
) {

    return new LoopEffect(new MaskEffect(
            new SolidEffect(PULSE_COLOR),
            (new SeriesLedPipeline)
                    ->addStage(new RandomWaitEffect(minWaitTime, maxWaitTime))
                    ->addStage(new RandomFadeInEffect(minFadeInTime, maxFadeInTime))
                    ->addStage(new RandomTimeBoxedEffect(new SolidEffect(CRGB::White), minFullEffectTime,
                                                         maxFullEffectTime, SamplingFunction::UNIFORM))
                    ->addStage(new RandomFadeOutEffect(minFadeOutTime, maxFadeOutTime)),
            true
    ));
}

void setup() {

    Serial.begin(115200);

    Serial.println("starting LEDs");

    FastLED.addLeds<WS2812B, HEADER_1_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 0, HEADER_LED_STRIP_LENGTH);
    FastLED.addLeds<WS2812B, HEADER_2_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 1, HEADER_LED_STRIP_LENGTH);
    FastLED.addLeds<WS2812B, HEADER_3_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 2, HEADER_LED_STRIP_LENGTH);
    FastLED.addLeds<WS2812B, HEADER_4_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 3, HEADER_LED_STRIP_LENGTH);
    FastLED.addLeds<WS2812B, HEADER_5_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 4, EXHAUST_LED_STRIP_LENGTH);

    LPLogger::initialize(LogLevel::LOG);
    FastLED.setMaxRefreshRate(60);
    FastLED.setBrightness(50);
    ledpipelines::initialize();
    ledpipelines::setMaxRefreshRate(60);

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);


    pipeline = new LoopEffect(
            (new ParallelLedPipeline(BlendingMode::ADD))
                    ->addStage(new SolidEffect(BASE_COLOR))
                    ->addStage(limitEffectToStrip(pulseEffect(0, 3, 2, 6, 3, 5, 10, 15), 0))
                    ->addStage(limitEffectToStrip(pulseEffect(0, 3, 2, 6, 3, 5, 10, 15), 1))
                    ->addStage(limitEffectToStrip(pulseEffect(0, 3, 2, 6, 3, 5, 10, 15), 2))
                    ->addStage(limitEffectToStrip(pulseEffect(0, 3, 2, 6, 3, 5, 10, 15), 3))
                    ->addStage(limitEffectToStrip(pulseEffect(0, 3, 2, 6, 3, 5, 10, 15), 4))
    );
    Serial.println("done initializing pipeline");
    pipeline->reset();
}

void loop() {
    pipeline->run();
}
