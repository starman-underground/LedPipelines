#include "Arduino.h"
#include "LedPipelines.h"

#define LED_PIN 5 // the pin we're attaching our LEDs to.

/**
 * LedPipelines depends on FastLED. To use FastLED, first declare all the LEDs that you would like to use. In this case,
 * we are declaring an array of 100 LEDs.
 */
CRGB leds[100];

/**
 * This object will store our LedPipeline once we initialize it.
 */
ledpipelines::LedPipeline *pipeline;


void setup() {

    /**
     * The first step is to add all your LEDs to FastLED. In this case, we have 100 WS2812B leds on pin LED_PIN.
     */
    FastLED.addLeds<WS2812B, LED_PIN, GBR>(leds, 100);

    /**
     * Always call initialize() after adding all the LEDs to FastLED. LedPipelines sets things up in the background
     * (such as counting all the leds and creating memory for the effects to follow) in this function. This can be
     * called before or after actually creating all your LedPipelines, but has to be called *after* adding all your
     * LEDs to FastLED.
     */
    ledpipelines::initialize();


    /**
     * An LedPipeline pipeline is a building block to build effects. LedPipelines are made up of multiple stages. Each
     * stage is its own effect, and can be anything - even another LedPipeline!. LedPipelines comes with a collection of
     * useful basic stages that can be added together and combined to create intricate effects out of the box, with full
     * customization.
     *
     * There are two basic types of LedPipelines - the ParallelLedPipeline and SeriesLedPipeline. Parallel pipelines
     * run all of their stages at the same time, layering them on top of one another. You can think of this like layers
     * in Photoshop. Series pipelines wait for each effect to finish before moving on to the next.
     *
     * Since we're only going to be adding a single stage, it doesn't matter which pipeline type we pick. We'll go with
     * ParallelLedPipeline to start with.
     */
    pipeline = new ledpipelines::ParallelLedPipeline();

    /**
     * The first (and only) effect that we'll add is a solid effect. Solid effects take in the color of the effect. In
     * this case, the color is red.
     */
     auto solidColorEffect = new ledpipelines::effects::SolidEffect(CRGB::Red);

    /**
     * When adding a stage to a pipeline, we use the addStage() method. LedPipelines take ownership of all the stages
     * that get added to them, and make sure they clean up all their resources. We pass in the effect that we created.
     * We could also do all of this in one line instead if we wanted to, but I've separated the effect to it's own
     * variable for clarity.
     *
     * Fun fact! addStage() returns a reference to the pipeline that you called it on, so you can chain multiple
     * addStage() calls together. You can see this in some of the other examples.
     */
     pipeline->addStage(solidColorEffect);
}


void loop() {
    /**
     * To run the effects in your LedPipeline, simply use the run() method on the pipeline. Call this method as often
     * as possible. It is blocking, and will also call FastLED.show();
     */
    pipeline->run();
}
