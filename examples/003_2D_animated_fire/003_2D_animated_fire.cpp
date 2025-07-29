#include "FastLED.h"
#include "LedPipelines.h"


using namespace ledpipelines;
using namespace ledpipelines::effects;


CRGB leds[256];

BaseLedPipelineStage *pipeline;

void setup() {


    Serial.begin(115200);

    Serial.println("starting LEDs");

    // Add the leds as normal.
    FastLED.addLeds<WS2812B, 38, GRB>(leds, 256);

    // Also create a LedLayout object that contains information on the order of LEDs in a LED matrix and its dimensions.
    // More information on this object and LED orderings at include/enums/LedLayout.h
    LedLayout layout = LedLayout(LedLayout::VERTICAL_SNAKE_XYFLIP, 32, 8);

    // Normal init stuff
    LPLogger::initialize(LogLevel::LOG);
    ledpipelines::initialize();
    ledpipelines::setMaxRefreshRate(60);
    FastLED.setBrightness(50);

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);

    // We can use the ImageEffect to create an animation pipeline by creating a series of time boxed image effects.
    // The following effect is of an 8x8 fire torch against a black background
    auto fireEffect1 = new LoopEffect(
        (new SeriesLedPipeline())
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame0, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame1, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame2, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame3, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame4, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame5, layout, 8, 8, 100), 0.05))
    );

    // Animating different fires with different frame counts/orderings makes the fire seem more realistic.
    auto fireEffect2 = new LoopEffect(
        (new SeriesLedPipeline())
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame1, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame0, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame3, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame2, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame4, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame5, layout, 8, 8, 100), 0.05))
    );
    auto fireEffect3 = new LoopEffect(
        (new SeriesLedPipeline())
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame0, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame4, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame3, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame1, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame4, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame2, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame2, layout, 8, 8, 100), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame5, layout, 8, 8, 100), 0.05))
    );
    
    pipeline = (new ParallelLedPipeline())
        ->addStage(new SolidEffect(CRGB::Black))
        // ImageEffects can be positioned using AbsolutePositionEffects, which can be used with 1D or 2D coordinates.
        ->addStage(new AbsolutePositionEffect(fireEffect1, 249.0f)) // Top left corner of a 32x8 VERTICAL_SNAKE_XYFLIP matrix
        // 2D coordinates require a LedLayout object to calculate the LED index from the coordinates.
        ->addStage(new AbsolutePositionEffect(fireEffect3, 24.0f, 0.0f, layout)) // Right side of the matrix
        // Stacking effects near each other can create larger fires.
        // Lowering some can make them seem shorter for more variation.
        ->addStage(new AbsolutePositionEffect(fireEffect1, 6.f, -2.0f, layout))
        ->addStage(new AbsolutePositionEffect(fireEffect3, 9.0f, 0.0f, layout))
        ->addStage(new AbsolutePositionEffect(fireEffect1, 10.0f, -3.0f, layout));

    Serial.println("done initializing pipeline");
    pipeline->reset();
}

void loop() {
    pipeline->run();
}
