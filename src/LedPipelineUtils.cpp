
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
    modified = new bool[size];
    for (int i = 0; i < size; i++) {
        (*this)[i] = CRGB::Black;
        modified[i] = false;
    }
}

TemporaryLedData::~TemporaryLedData() {
    delete[] data;
    delete[] modified;
}

void TemporaryLedData::merge(TemporaryLedData &other, BlendingMode blendingMode) {
    for (int i = 0; i < TemporaryLedData::size; i++) {
        // if other pixel isn't modified, we skip this pixel.
        if (!other.modified[i])
            continue;

        this->modified[i] = true;
        switch (blendingMode) {
            case OVERWRITE:
                this->data[i] = other[i];
                break;
            case ADD:
                this->data[i] = other[i] + this->data[i];
                break;
            case MULTIPLY:
                this->data[i] *= other[i] * this->data[i];
                break;
        }
    }
}

void TemporaryLedData::set(int index, CRGB &color) {
    modified[index] = true;
    (*this)[index] = color;
}

void TemporaryLedData::set(int stripIndex, int ledIndex, CRGB &color) {
    if (stripIndex < 0 || stripIndex >= FastLED.count()) return; // strip doesn't exist
    if (ledIndex < 0 || ledIndex >= FastLED[stripIndex].size()) return; // LED doesn't exist on specified strip
    int index = startIndexes[stripIndex] + ledIndex; // index in array
    this->set(index, color);
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
    String data = "R: \t";
    for (int i = 0; i < size; i++) {
        data += this->data[i].r;
        data += "\t";
    }
    LPLogger::log(data);
    data = "G: \t";
    for (int i = 0; i < size; i++) {
        data += this->data[i].g;
        data += "\t";
    }
    LPLogger::log(data);
    data = "B: \t";
    for (int i = 0; i < size; i++) {
        data += this->data[i].b;
        data += "\t";
    }
    LPLogger::log(data);
    data = "M: \t";
    for (int i = 0; i < size; i++) {
        data += this->modified[i];
        data += "\t";
    }
    LPLogger::log(data + "\n");
}
