#include "FastLED.h"
#include "LedPipelines.h"


LedPipeline *pipeline;

void setup() {
    CRGB *leds = new CRGB[1000];

    Serial.begin(115200);

    Serial.println("starting LEDs");

    FastLED.addLeds<WS2812B, 12, GRB>(leds, 20);

    FastLED.showColor(CRGB::Black);


    LedPipelinesUtils::setLogLevel(VERBOSE);

    FastLED.show();

    delay(1000);
    FastLED.setMaxRefreshRate(30);


    TemporaryLedData::initialize();

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);

    pipeline = ((new ParallelLedPipeline())
            ->addStage(new SolidEffect(CRGB::White))
            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Red, 5), 0))
            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Green, 5), 5))
            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Blue, 10), 10))
    );
    Serial.println("done initializing pipeline");
}

void loop() {
    pipeline->run();
}
