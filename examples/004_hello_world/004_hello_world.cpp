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
    // Change the layout to match your LED matrix configuration.
    // Try this and make the following changes depending on your observations:
    // upside down -> YFLIP the layout
    // mirrored text -> XFLIP the layout
    // rotate the text 90 degrees clockwise -> change VERTICAL to HORIZONTAL
    // NOTE: Changes from VERTICAL to HORIZONTAL will also require switching the width and height parameters.
    // More information on this object and LED orderings at include/enums/LedLayout.h
    LedLayout layout = LedLayout(LedLayout::VERTICAL_SNAKE_XYFLIP, 32, 8);

    // Normal init stuff
    LPLogger::initialize(LogLevel::LOG);
    ledpipelines::initialize();
    ledpipelines::setMaxRefreshRate(60);
    FastLED.setBrightness(50);

    Serial.print("There are this many leds: ");
    Serial.println(TemporaryLedData::size);
    
    // TextEffect is a simple effect that displays text on the LEDs.
    // It can be positioned using the AbsolutePositionEffect in the same way as the ImageEffect.
    // NOTE: The TextEffect regards the Y coordinate as the bottom of the text, which is the opposite of the ImageEffect.
    // The MovingEffect is used here to move the TextEffect across the LEDs using 2D coordinates.
    // A MovingEffect with a TextEffect can be used to create scrolling text effects as shown below.
    pipeline = (new ParallelLedPipeline())
        ->addStage(new SolidEffect(CRGB::Black))
        ->addStage(new LoopEffect( // With a loop effect, we can have the text scroll indefinitely.
            (new ParallelLedPipeline())
                ->addStage(new MovingEffect(
                    new TextEffect("Hello World!", layout, CRGB::DarkViolet, CRGB::DarkSlateBlue, 128),
                    4.0f,
                    layout,
                    {1.0f, 7.0f}, // start position (bottom left corner)
                    {-79.0f, 7.0f}, // end position (far left off screen)
                    SmoothingFunction(SmoothingFunction::SMOOTH_LINEAR)
                ))
                ->addStage(new MovingEffect( // Add another TextEffect to come in from the right as the first one goes out.
                    new TextEffect("Hello World!", layout, CRGB::DarkViolet, CRGB::DarkSlateBlue, 128),
                    4.0f,
                    layout,
                    {81.0f, 7.0f}, // start position (far right off screen)
                    {1.0f, 7.0f}, // end position (bottom left corner)
                    SmoothingFunction(SmoothingFunction::SMOOTH_LINEAR)
                ))
        ));
        
    Serial.println("done initializing pipeline");
    pipeline->reset();
}

void loop() {
    pipeline->run();
}
