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

BaseLedPipelineStage *pipeline;

CRGB leds[1000];

unsigned long lastFpsPrintTime = millis();

BaseLedPipelineStage *pulseEffect(CRGB color, float fadeInTime, float pauseTime, float fadeOutTime) {
    return new MaskEffect(
            new TimeBoxedEffect(new SolidEffect(color), (fadeInTime + pauseTime + fadeOutTime)),
            (new SeriesLedPipeline())
                    ->addStage(new FadeInEffect(fadeInTime))
                    ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::White), pauseTime))
                    ->addStage(new FadeOutEffect(fadeOutTime))
                    ->addStage(new SolidEffect(CRGB::White, 0)),
            true
    );
}

void setup() {

    Serial.begin(115200);

    Serial.println("starting LEDs");

    FastLED.addLeds<WS2812B, HEADER_1_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 0, HEADER_LED_STRIP_LENGTH);
    FastLED.addLeds<WS2812B, HEADER_2_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 1, HEADER_LED_STRIP_LENGTH);
    FastLED.addLeds<WS2812B, HEADER_3_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 2, HEADER_LED_STRIP_LENGTH);
    FastLED.addLeds<WS2812B, HEADER_4_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 3, HEADER_LED_STRIP_LENGTH);
    FastLED.addLeds<WS2812B, HEADER_5_PIN, GRB>(leds, HEADER_LED_STRIP_LENGTH * 4, EXHAUST_LED_STRIP_LENGTH);

    LPLogger::setLogLevel(Debug);

//    FastLED.setMaxRefreshRate(60);
//    FastLED.setBrightness(50);
    ledpipelines::initialize();

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);
    const CRGB color = 0xFF2000;

    pipeline = new LoopEffect(
            (new SeriesLedPipeline)
                    ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::Red), 2))
                    ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::Green), 2))
                    ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::Blue), 2))
    );
    Serial.println("done initializing pipeline");
    pipeline->reset();
}

void loop() {
    unsigned long startTime = millis();
    pipeline->run();
    unsigned long endTime = millis();
    unsigned long frameRate = 1000.0f / (endTime - startTime);
    if ((endTime - lastFpsPrintTime) >= 1000) {
        lastFpsPrintTime = endTime;
        LPLogger::log(String("framerate: ") + frameRate);
    }
}
