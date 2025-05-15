#pragma once

#include "FastLED.h"

#include "enums/BlendingMode.h"
#include "enums/SmoothingFunction.h"
#include "enums/SamplingFunction.h"
#include "TemporaryLedData.h"
#include "LedPipelinesLogger.h"


CRGB operator*(CRGB first, CRGB second);

CRGB &operator*=(CRGB &first, const CRGB &second);

namespace ledpipelines {




/**
 * Convert a Color and opacity to a hex code, in form FFFFFFFF, or RGBA.
 * @param color the color to convert, in RGB form.
 * @param opacity the opacity of the color to convert.
 * @return the color in Hex form in a string.
 */
String colorToHex(CRGB color, uint8_t opacity);




}