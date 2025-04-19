#include "FastLED.h"
#include "LedPipelines.h"


LedPipeline *pipeline;

void setup() {

    CRGB *leds = new CRGB[1000];

    Serial.begin(115200);

    Serial.println("starting LEDs");

    FastLED.addLeds<WS2812B, 12, GRB>(leds, 20);

    LPLogger::setLogLevel(Debug);

    FastLED.setMaxRefreshRate(60);
    FastLED.setBrightness(50);
    TemporaryLedData::initialize();

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);
    const CRGB orange = 0xFF2000;

    pipeline = (new ParallelLedPipeline())
            ->addStage(
                    new MaskEffect(
                            (new ParallelLedPipeline)
                                    ->addStage(new SolidEffect(orange))
                                    ->addStage(
                                            new LoopEffect(
                                                    new MovingEffect(
                                                            new RepeatEffect(
                                                                    new SolidSegmentEffect(
                                                                            CRGB::White, 5
                                                                    ),
                                                                    10),
                                                            10,
                                                            0,
                                                            20
                                                    ),
                                                    2
                                            )
                                    ),
                            new LoopEffect(
                                    (new SeriesLedPipeline())
                                            ->addStage(new FadeInEffect(
                                                    1
                                            ))
                                            ->addStage(new FadeOutEffect(
                                                    2
                                            ))),
                            true
                    )
            )
//            ->addStage(new SolidSegmentEffect(CRGB::Blue, 50))
//            ->addStage(
//                    new LoopEffect(
//                            new MovingEffect(
//                                    new RepeatEffect(
//                                            new SolidSegmentEffect(
//                                                    CRGB::White, 5
//                                            ),
//                                            10),
//                                    10,
//                                    0,
//                                    20
//                            ),
//                            2
//                    )
//            )

            ;













































//    pipeline = ((new ParallelLedPipeline())
////            ->addStage(new SolidEffect(CRGB::Black))
////            ->addStage(new LoopEffect(
////                    (new SeriesLedPipeline(BlendingMode::OVERWRITE))
////                            ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::White), 1))
////                            ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::Yellow), 2))
////                            ->addStage(new TimeBoxedEffect(new SolidEffect(CRGB::Magenta), 3)))
////            )
////            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Red, 5), 0))
////            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Green, 5), 5))
////            ->addStage(new OffsetEffect(new SolidSegmentEffect(CRGB::Blue, 5), 10))

//            ->addStage(
//                    new LoopEffect(
//                            new MovingEffect(
//                                    new RepeatEffect(
//                                            (new ParallelLedPipeline())
//                                                    ->addStage(
//                                                            new SolidSegmentEffect(
//                                                                    CRGB::White,
//                                                                    10
//                                                            )
//                                                    )
//                                                    ->addStage(
//                                                            new OpacityGradientEffect(
//                                                                    new OpacityGradientEffect(
//                                                                            new SolidSegmentEffect(
//                                                                                    CRGB::DarkRed, 10
//                                                                            ),
//                                                                            -5,
//                                                                            10),
//                                                                    5
//                                                            )
//                                                    )
//                                            ,
//                                            20),
//                                    -5,
//                                    0,
//                                    -20
//                            )
//                    )
//            )
//    );
    Serial.println("done initializing pipeline");
    pipeline->reset();
}

void loop() {
    unsigned long startTime = millis();
    pipeline->run();
    unsigned long endTime = millis();
    unsigned long frameRate = 1000.0f / (endTime - startTime);
    LPLogger::log(String("framerate: ") + frameRate);
}
