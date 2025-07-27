#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

// Range of available ASCII characters
constexpr int FIRST_CHAR = 32;
constexpr int LAST_CHAR = 126;
constexpr int NUM_CHARS = LAST_CHAR - FIRST_CHAR + 1;

// Glyph metrics structure
struct GlyphMetrics {
    uint8_t width;
    uint8_t height;
    int8_t xOffset; // Horizontal offset from cursor
    int8_t yOffset;
    uint8_t advanceWidth;
    uint16_t dataOffset; // Offset in bits from start of bitmap data
};

class BitPacker {
private:
    std::vector<uint8_t> data;
    int bitPosition;

public:
    BitPacker() : bitPosition(0) {}

    void addBit(bool bit) {
        // Ensure we have enough space
        int byteIndex = bitPosition / 8;
        if (byteIndex >= data.size()) {
            data.resize(byteIndex + 1, 0);
        }
        
        if (bit) {
            int bitOffset = 7 - (bitPosition % 8); // MSB first
            data[byteIndex] |= (1 << bitOffset);
        }
        bitPosition++;
    }

    const std::vector<uint8_t>& getData() const { return data; }
    int getBitCount() const { return bitPosition; }
    
    // Get current bit position for offset tracking
    int getCurrentOffset() const { return bitPosition; }
};

bool extractPixel(unsigned char* bitmap, int width, int height, int x, int y) {
    if (x >= width || y >= height || x < 0 || y < 0) return false;
    return bitmap[y * width + x] > 128; // Threshold for "on" pixel
}

void generateOptimizedHeader(const std::vector<uint8_t>& packedData, 
                           const std::vector<GlyphMetrics>& metrics,
                           int maxHeight) {
    std::ofstream header("../include/resources/FontAtlas.h");
    
    header << "#pragma once\n";
    header << "#include <Arduino.h>\n";
    header << "#include <stdint.h>\n\n";
    
    // Font constants
    header << "// Optimized Variable-Width Font Atlas Configuration\n";
    header << "constexpr int FIRST_ASCII = " << FIRST_CHAR << ";\n";
    header << "constexpr int LAST_ASCII = " << LAST_CHAR << ";\n";
    header << "constexpr int NUM_CHARACTERS = " << NUM_CHARS << ";\n";
    header << "constexpr int FONT_MAX_HEIGHT = " << maxHeight << ";\n\n";
    
    // Glyph metrics structure
    header << "struct GlyphMetrics {\n";
    header << "    uint8_t width;\n";
    header << "    uint8_t height;\n";
    header << "    int8_t xOffset;\n";
    header << "    int8_t yOffset;\n";
    header << "    uint8_t advanceWidth;\n";
    header << "    uint16_t dataOffset; // Offset in bits\n";
    header << "};\n\n";
    
    // Glyph metrics array
    header << "const GlyphMetrics GLYPH_METRICS[] PROGMEM = {\n";
    for (size_t i = 0; i < metrics.size(); ++i) {
        const auto& m = metrics[i];
        header << "    {" << static_cast<int>(m.width) << ", " 
               << static_cast<int>(m.height) << ", "
               << static_cast<int>(m.xOffset) << ", "
               << static_cast<int>(m.yOffset) << ", "
               << static_cast<int>(m.advanceWidth) << ", "
               << m.dataOffset << "}";
        if (i < metrics.size() - 1) header << ",";
        header << " // '" << static_cast<char>(FIRST_CHAR + i) << "'\n";
    }
    header << "};\n\n";
    
    // Packed bitmap data array
    header << "const uint8_t COMPRESSED_FONT_DATA[] PROGMEM = {\n";
    for (size_t i = 0; i < packedData.size(); ++i) {
        if (i % 16 == 0) header << "    ";
        header << "0x" << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(packedData[i]);
        if (i < packedData.size() - 1) header << ",";
        if (i % 16 == 15) header << "\n";
    }
    header << "\n};\n\n";
    
    // Helper functions
    header << "// Helper function to get glyph metrics (undefined behavior if char is out of range: ['" << static_cast<char>(FIRST_CHAR) << "' (" << FIRST_CHAR << "), '" << static_cast<char>(LAST_CHAR) << "' (" << std::dec << LAST_CHAR << ")])\n";
    header << "inline const GlyphMetrics& getGlyphMetrics(char c) {\n";
    header << "    return GLYPH_METRICS[c - FIRST_ASCII];\n";
    header << "}\n\n";
    
    header << "// Helper function to extract glyph bitmap\n";
    header << "// Returns true if pixel at (x,y) is set, false otherwise\n";
    header << "inline bool getGlyphPixel(char c, int x, int y) {\n";
    header << "    const GlyphMetrics& metrics = getGlyphMetrics(c);\n";
    header << "    if (x >= metrics.width || y >= metrics.height || x < 0 || y < 0) {\n";
    header << "        return false;\n";
    header << "    }\n";
    header << "    \n";
    header << "    int bitOffset = metrics.dataOffset + (y * metrics.width + x);\n";
    header << "    int byteIndex = bitOffset / 8;\n";
    header << "    int bitPosition = 7 - (bitOffset % 8);\n";
    header << "    \n";
    header << "    return (COMPRESSED_FONT_DATA[byteIndex] & (1 << bitPosition)) != 0;\n";
    header << "}\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <font.ttf>\n";
        std::cerr << "Generates optimized variable-width bitmap font atlas from .ttf file at include/resources/FontAtlas.h.\n";
        return 1;
    }

    // Load TTF file
    std::ifstream fontFile(argv[1], std::ios::binary | std::ios::ate);
    if (!fontFile) {
        std::cerr << "Failed to open font file: " << argv[1] << "\n";
        return 1;
    }

    auto fileSize = fontFile.tellg();
    std::vector<uint8_t> fontBuffer(fileSize);
    fontFile.seekg(0, std::ios::beg);
    fontFile.read(reinterpret_cast<char*>(fontBuffer.data()), fileSize);
    fontFile.close();

    // Initialize font
    stbtt_fontinfo font;
    if (!stbtt_InitFont(&font, fontBuffer.data(), 0)) {
        std::cerr << "Failed to initialize font\n";
        return 1;
    }

    // Determine optimal font size based on target height
    int targetHeight = 8; // You can adjust this
    float scale = stbtt_ScaleForPixelHeight(&font, targetHeight);
    
    BitPacker packer;
    std::vector<GlyphMetrics> metrics;
    metrics.reserve(NUM_CHARS);
    
    int maxHeight = 0;
    int totalBits = 0;

    std::cout << "Processing " << NUM_CHARS << " characters...\n";
    std::cout << "Target height: " << targetHeight << " pixels\n";

    // First pass: calculate metrics and determine actual font dimensions
    for (int c = FIRST_CHAR; c <= LAST_CHAR; ++c) {
        int glyph = stbtt_FindGlyphIndex(&font, c);
        
        // Get glyph metrics
        int advanceWidth, leftSideBearing;
        stbtt_GetGlyphHMetrics(&font, glyph, &advanceWidth, &leftSideBearing);
        
        // Get bitmap
        int width, height, xoff, yoff;
        unsigned char* bitmap = stbtt_GetGlyphBitmap(
            &font, scale, scale, glyph, &width, &height, &xoff, &yoff);
        
        // Handle empty glyphs (like space)
        if (!bitmap || width == 0 || height == 0) {
            width = 1;
            height = 1;
        }
        
        GlyphMetrics gm;
        gm.width = std::min(255, std::max(1, width));
        gm.height = std::min(255, std::max(1, height));
        gm.xOffset = std::max(-128, std::min(127, xoff));
        gm.yOffset = std::max(-128, std::min(127, yoff));
        gm.advanceWidth = std::min(255, static_cast<int>(advanceWidth * scale));
        gm.dataOffset = totalBits;
        
        metrics.push_back(gm);
        
        maxHeight = std::max(maxHeight, static_cast<int>(gm.height));
        totalBits += gm.width * gm.height;
        
        if (bitmap) {
            stbtt_FreeBitmap(bitmap, nullptr);
        }
    }

    // Second pass: pack the actual bitmap data
    std::cout << "Packing bitmap data...\n";
    for (int c = FIRST_CHAR; c <= LAST_CHAR; ++c) {
        int charIndex = c - FIRST_CHAR;
        int glyph = stbtt_FindGlyphIndex(&font, c);
        
        int width, height, xoff, yoff;
        unsigned char* bitmap = stbtt_GetGlyphBitmap(
            &font, scale, scale, glyph, &width, &height, &xoff, &yoff);
        
        const GlyphMetrics& gm = metrics[charIndex];
        
        // Pack pixels in row-major order (y, then x)
        for (int y = 0; y < gm.height; ++y) {
            for (int x = 0; x < gm.width; ++x) {
                bool pixelOn = false;
                if (bitmap && width > 0 && height > 0) {
                    pixelOn = extractPixel(bitmap, width, height, x, y);
                }
                packer.addBit(pixelOn);
            }
        }
        
        if (bitmap) {
            stbtt_FreeBitmap(bitmap, nullptr);
        }
        
        // Progress indicator
        if ((charIndex + 1) % 10 == 0) {
            std::cout << "Processed " << (charIndex + 1) << "/" << NUM_CHARS << " characters\n";
        }
    }

    // Generate header file
    generateOptimizedHeader(packer.getData(), metrics, maxHeight);
    
    // Calculate space savings compared to fixed-size approach
    int fixedSizeBytes = NUM_CHARS * 5; // Original 5x8 = 40 bits per char = 5 bytes
    int actualBytes = packer.getData().size();
    
    std::cout << "\nOptimization Results:\n";
    std::cout << "Total characters: " << NUM_CHARS << "\n";
    std::cout << "Max height: " << maxHeight << " pixels\n";
    std::cout << "Total bits packed: " << packer.getBitCount() << "\n";
    std::cout << "Compressed size: " << actualBytes << " bytes\n";
    std::cout << "Fixed 5x8 size would be: " << fixedSizeBytes << " bytes\n";
    std::cout << "Space savings: " << std::fixed << std::setprecision(1)
              << (100.0 * (1.0 - static_cast<double>(actualBytes) / fixedSizeBytes))
              << "%\n";
    std::cout << "Font atlas generated successfully at include/resources/FontAtlas.h!\n";

    return 0;
}
