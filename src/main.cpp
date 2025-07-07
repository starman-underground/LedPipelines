#include "FastLED.h"
#include "LedPipelines.h"


using namespace ledpipelines;
using namespace ledpipelines::effects;


CRGB leds[100];

BaseLedPipelineStage *pipeline;

void setup() {

    Serial.begin(115200);

    Serial.println("starting LEDs");

    FastLED.addLeds<WS2812B, 14, RGB>(leds, 30);

    LPLogger::initialize(LogLevel::LOG);
    FastLED.setMaxRefreshRate(60);

    ledpipelines::initialize();
    ledpipelines::setMaxRefreshRate(60);

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);

    pipeline = new LoopEffect(
            new MovingEffect(
                    new SolidSegmentEffect(
                            CRGB::Red,
                            2.5
                    ),
                    5, 0, 10
            )
    );

    Serial.println("done initializing pipeline");
    pipeline->reset();
}

void loop() {
    pipeline->run();
}
