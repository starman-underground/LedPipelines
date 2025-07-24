#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

// Font configuration
constexpr int FONT_W = 3;
constexpr int FONT_H = 5;
constexpr int FIRST_CHAR = 32;
constexpr int LAST_CHAR = 126;
constexpr int NUM_CHARS = LAST_CHAR - FIRST_CHAR + 1;
constexpr int BITS_PER_CHAR = FONT_W * FONT_H;
constexpr int TOTAL_BITS = NUM_CHARS * BITS_PER_CHAR;
constexpr int TOTAL_BYTES = (TOTAL_BITS + 7) / 8; // Round up to nearest byte

class BitPacker {
private:
    std::vector<uint8_t> data;
    int bitPosition;

public:
    BitPacker() : data((TOTAL_BITS + 7) / 8, 0), bitPosition(0) {}
    
    void addBit(bool bit) {
        if (bit) {
            int byteIndex = bitPosition / 8;
            int bitOffset = 7 - (bitPosition % 8);  // MSB first
            data[byteIndex] |= (1 << bitOffset);
        }
        bitPosition++;
    }
    
    const std::vector<uint8_t>& getData() const { return data; }
    int getBitCount() const { return bitPosition; }
};

bool extractPixel(unsigned char* bitmap, int width, int height, int x, int y) {
    if (x >= width || y >= height || x < 0 || y < 0) return false;
    return bitmap[y * width + x] > 128;  // Threshold for "on" pixel
}

void generateCompressedHeader(const std::vector<uint8_t>& packedData, const std::string& filename) {
    std::ofstream header(filename);
    
    header << "#pragma once\n";
    header << "#include <Arduino.h>\n\n";
    
    // Font constants
    header << "// Compressed 3x5 Font Atlas Configuration\n";
    header << "constexpr int FONT_WIDTH = " << FONT_W << ";\n";
    header << "constexpr int FONT_HEIGHT = " << FONT_H << ";\n";
    header << "constexpr int FIRST_ASCII = " << FIRST_CHAR << ";\n";
    header << "constexpr int LAST_ASCII = " << LAST_CHAR << ";\n";
    header << "constexpr int NUM_CHARACTERS = " << NUM_CHARS << ";\n";
    header << "constexpr int BITS_PER_CHARACTER = " << BITS_PER_CHAR << ";\n\n";
    
    // Packed data array
    header << "const uint8_t COMPRESSED_FONT_DATA[] PROGMEM = {\n";
    for (size_t i = 0; i < packedData.size(); ++i) {
        if (i % 16 == 0) header << "  ";
        header << "0x" << std::hex << std::setw(2) << std::setfill('0') 
               << static_cast<int>(packedData[i]);
        if (i < packedData.size() - 1) header << ",";
        if (i % 16 == 15) header << "\n";
    }
    header << "\n};\n\n";
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <font.ttf> <output.h>\n";
        std::cerr << "Generates compressed bitmap font atlas for 3x5 pixel characters\n";
        return 1;
    }

    // Load TTF file
    std::ifstream fontFile(argv[1], std::ios::binary | std::ios::ate);
    if (!fontFile) {
        std::cerr << "Failed to open font file: " << argv[1] << "\n";
        return 1;
    }
    
    auto fileSize = fontFile.tellg();
    std::vector<unsigned char> fontBuffer(fileSize);
    fontFile.seekg(0, std::ios::beg);
    fontFile.read(reinterpret_cast<char*>(fontBuffer.data()), fileSize);
    fontFile.close();

    // Initialize font
    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, fontBuffer.data(), 0)) {
        std::cerr << "Failed to initialize font\n";
        return 1;
    }

    BitPacker packer;
    float scale = stbtt_ScaleForPixelHeight(&font, FONT_H);

    std::cout << "Processing " << NUM_CHARS << " characters...\n";
    std::cout << "Target size: " << FONT_W << "x" << FONT_H << " pixels per character\n";

    // Process each character
    for (int c = FIRST_CHAR; c <= LAST_CHAR; ++c) {
        int glyph = stbtt_FindGlyphIndex(&font, c);
        
        int width, height, xoff, yoff;
        unsigned char* bitmap = stbtt_GetGlyphBitmap(
            &font, scale, scale, glyph, &width, &height, &xoff, &yoff);

        // Pack pixels in row-major order (y, then x)
        for (int y = 0; y < FONT_H; ++y) {
            for (int x = 0; x < FONT_W; ++x) {
                bool pixelOn = extractPixel(bitmap, width, height, x, y);
                packer.addBit(pixelOn);
            }
        }

        stbtt_FreeBitmap(bitmap, nullptr);
        
        // Progress indicator
        if ((c - FIRST_CHAR + 1) % 10 == 0) {
            std::cout << "Processed " << (c - FIRST_CHAR + 1) << "/" << NUM_CHARS << " characters\n";
        }
    }

    // Generate header file
    generateCompressedHeader(packer.getData(), argv[2]);
    
    std::cout << "\nCompression Results:\n";
    std::cout << "Total bits used: " << packer.getBitCount() << "/" << TOTAL_BITS << "\n";
    std::cout << "Compressed size: " << packer.getData().size() << " bytes\n";
    std::cout << "Standard format would be: " << (NUM_CHARS * 5) << " bytes\n";
    std::cout << "Space savings: " << std::fixed << std::setprecision(1) 
              << (100.0 * (1.0 - static_cast<double>(packer.getData().size()) / (NUM_CHARS * 5))) 
              << "%\n";
    std::cout << "Header generated: " << argv[2] << "\n";

    return 0;
}
