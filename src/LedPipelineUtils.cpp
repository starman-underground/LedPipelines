
#include "LedPipelineUtils.h"


LedPipelinesLogLevel LPLogger::logLevel = Debug;

void LPLogger::logInternal(LedPipelinesLogLevel logLevel, String &log) {

    if (LPLogger::logLevel >= logLevel) {
        String logLevelString;
        switch (logLevel) {
            case NONE:
                logLevelString = "[NON]";
                break;
            case ERROR:
                logLevelString = "[ERR]";
                break;
            case WARN:
                logLevelString = "[WAR]";
                break;
            case LOG:
                logLevelString = "[LOG]";
                break;
            case Debug:
                logLevelString = "[DEB]";
                break;
        }
        Serial.println(logLevelString + " " + log);
    }
}

int interpolate(LedPipelinesSmoothingType type, int oldStart, int oldEnd, int newStart, int newEnd, int amount) {
    float floatAmount = ((float) (amount - oldStart) / (float) (oldEnd - oldStart));
    return interpolate(type, newStart, newEnd, floatAmount);
}


int interpolate(LedPipelinesSmoothingType type, int newStart, int newEnd, float amount) {
    switch (type) {
        case LINEAR:
            // f(x) = x, so we don't do anything.
            break;
        case SINE:
            // f(x) = sin(pi * x / 2)
            amount = sin(PI * amount / 2);
            break;
        case SMOOTH_LINEAR:
            // f(x) = 0.5 * cos(pi * x + pi) + 0.5
            amount = 0.5 * cos(PI * amount + PI) + 0.5;
            break;
    }
    return (int) (newStart + (newEnd - newStart) * amount);
}

CRGB operator*(const CRGB first, const CRGB second) {
    return CRGB(
            ((size_t) first.r * (size_t) second.r) / 255,
            ((size_t) first.g * (size_t) second.g) / 255,
            ((size_t) first.b * (size_t) second.b) / 255
    );
}

CRGB &operator*=(CRGB &first, const CRGB &second) {
    first.r = ((size_t) first.r * (size_t) second.r) / 255;
    first.g = ((size_t) first.g * (size_t) second.g) / 255;
    first.b = ((size_t) first.b * (size_t) second.b) / 255;
    return first;
}

static char HexLookUp[] = "0123456789abcdef";

static String byteToHex(uint8_t num) {
    uint8_t firstDigit = (num / 0x10);
    uint8_t secondDigit = (num % 0x10);
    return String(HexLookUp[firstDigit]) + HexLookUp[secondDigit];
}

String colorToHex(CRGB color, uint8_t opacity) {
    return String(byteToHex(color.r) + byteToHex(color.g) + byteToHex(color.b) + byteToHex(opacity));
}


int TemporaryLedData::size = 0;

int *TemporaryLedData::startIndexes = nullptr;

void TemporaryLedData::initialize() {

    // calculate the total number of LEDs.
    TemporaryLedData::startIndexes = new int[FastLED.count()];
    size = 0;
    for (int i = 0; i < FastLED.count(); i++) {
        TemporaryLedData::startIndexes[i] = size;
        size += FastLED[i].size();
    }
}

TemporaryLedData::TemporaryLedData() {
    data = new CRGB[size];
    opacity = new uint8_t[size];
    for (int i = 0; i < size; i++) {
        (*this)[i] = CRGB::Black;
        opacity[i] = false;
    }
}


TemporaryLedData::~TemporaryLedData() {
    delete[] data;
    delete[] opacity;
}

void TemporaryLedData::merge(TemporaryLedData &other, BlendingMode blendingMode) {
    for (int i = 0; i < TemporaryLedData::size; i++) {
        auto A_alpha = this->opacity[i];
        auto A_rgb = this->data[i];
        auto B_alpha = other.opacity[i];
        auto B_rgb = other.data[i];
        // if other pixel has no opacity, we skip this pixel.
        if (!other.opacity[i])
            continue;

        this->anyAreModified = true;
        switch (blendingMode) {
            case OVERWRITE:
                this->data[i] = B_rgb;
                this->opacity[i] = B_alpha;
                break;
            case ADD:
                this->data[i] = A_rgb + B_rgb;
                this->opacity[i] = min(A_alpha + B_alpha, UINT8_MAX);
                break;
            case MULTIPLY:
                this->data[i] *= A_rgb * B_rgb;
                this->opacity[i] = (B_alpha * B_alpha) / 255;
                break;
            case NORMAL:
                this->data[i].r = ((255 - B_alpha) * A_rgb.r + B_alpha * B_rgb.r) / 255;
                this->data[i].g = ((255 - B_alpha) * A_rgb.g + B_alpha * B_rgb.g) / 255;
                this->data[i].b = ((255 - B_alpha) * A_rgb.b + B_alpha * B_rgb.b) / 255;
                this->opacity[i] = B_alpha + ((255 - B_alpha) * A_alpha) / 255;
                break;
        }
    }
}

//void TemporaryLedData::shift(int shiftAmount) {
//    if (shiftAmount < 0) {
//        // since it's less than 0, we lose the information on the start. So it's okay to override the information at the
//        // beginning.
//        for (int i = 0; i < TemporaryLedData::size; i++) {
//            this->set(i, this->data[i + shiftAmount], this->opacity[i + shiftAmount]);
//        }
//
//        // we reset the data at the end of the buffer to 0 / black.
//        for (int i = TemporaryLedData::size - shiftAmount; i < TemporaryLedData::size; i++) {
//            this->set(i, CRGB::Black, 0);
//        }
//    } else {
//        // since it's less than 0, we lose the information on the start. So it's okay to override the information at the
//        // beginning.
//        for (int i = TemporaryLedData::size - 1; i >= shiftAmount; i--) {
//            this->set(i, this->data[i + shiftAmount], this->opacity[i + shiftAmount]);
//        }
//
//        // we reset the data at the end of the buffer to 0 / black.
//        for (int i = 0; i < shiftAmount; i++) {
//            this->set(i, CRGB::Black, 0);
//        }
//    }
//}


void TemporaryLedData::set(int index, CRGB color, uint8_t opacity) {
    if (index < 0 || index >= TemporaryLedData::size) return; // LED doesn't exist on specified strip
    this->opacity[index] = opacity;
    anyAreModified = true;
    (*this)[index] = color;
}

void TemporaryLedData::set(int stripIndex, int ledIndex, CRGB &color, uint8_t opacity) {
    if (stripIndex < 0 || stripIndex >= FastLED.count()) return; // strip doesn't exist
    int index = startIndexes[stripIndex] + ledIndex; // index in array
    this->set(index, color, opacity);
}

CRGB TemporaryLedData::get(int index) const {
    if (index < 0 || index >= size)
        return CRGB::Black;

    return this->data[index];
}

uint8_t TemporaryLedData::getOpacity(int index) const {
    if (index < 0 || index >= size)
        return 0;

    return this->opacity[index];
}

void TemporaryLedData::populateFastLed() const {
    int currentLed = 0;
    for (int i = 0; i < FastLED.count(); i++) {
        for (int j = 0; j < FastLED[i].size(); (j++, currentLed++)) {
            FastLED[i][j] = (*this)[currentLed];
        }
    }
}

void TemporaryLedData::printData() const {
    String data = "";
    for (int i = 0; i < size; i++) {
        data += colorToHex(this->data[i], this->opacity[i]) + " ";
    }
    LPLogger::log(data);
}
