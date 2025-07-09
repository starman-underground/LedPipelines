#include "FastLED.h"
#include "LedPipelines.h"


using namespace ledpipelines;
using namespace ledpipelines::effects;


CRGB leds[100];

BaseLedPipelineStage *pipeline;

void setup() {

    Serial.begin(115200);

    Serial.println("starting LEDs");

    FastLED.addLeds<WS2812B, 14, RGB>(leds, 100);

    LPLogger::initialize(LogLevel::LOG);

    ledpipelines::initialize();
    ledpipelines::setMaxRefreshRate(60);
    FastLED.setBrightness(50);

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);


    pipeline = (new ParallelLedPipeline())
            ->addStage(new SolidEffect(CRGB::White))
            ->addStage(
                    new LoopEffect(
                            (new SeriesLedPipeline())
                                    ->addStage(
                                            new MovingEffect(
                                                    new SolidSegmentEffect(
                                                            CRGB::Red,
                                                            2
                                                    ),
                                                    1, 0, 20, SmoothingFunction::INVERSE_QUADRATIC
                                            )
                                    )
                                    ->addStage(
                                            new MovingEffect(
                                                    new SolidSegmentEffect(
                                                            CRGB::Red,
                                                            2
                                                    ),
                                                    1, 20, 0, SmoothingFunction::QUADRATIC
                                            )
                                    )
                    )
            );

    Serial.println("done initializing pipeline");
    pipeline->reset();
}

void loop() {
    pipeline->run();
}
