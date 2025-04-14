#include "FastLED.h"
#include "LedPipelines.h"


LedPipeline *pipeline;

void setup() {
    CRGB *leds = new CRGB[1000];

    Serial.begin(115200);

    Serial.println("starting LEDs");

    FastLED.addLeds<WS2812B, 12, GRB>(leds, 20);

    FastLED.showColor(CRGB::Black);


    LPLogger::setLogLevel(Debug);

    FastLED.show();

    delay(1000);
    FastLED.setMaxRefreshRate(30);


    TemporaryLedData::initialize();

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);

    pipeline = ((new ParallelLedPipeline())
            ->addStage(new LoopEffect(
                    (new SeriesLedPipeline())
                            ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::White), 1))
                            ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::Yellow), 2))
                            ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::Magenta), 3)))
            )
            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Red, 5), 0))
            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Green, 5), 5))
            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Blue, 5), 10))
    );
    Serial.println("done initializing pipeline");
    pipeline->reset();
}

void loop() {
    pipeline->run();
}
