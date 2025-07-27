#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;      // 'BM'
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

class BMPSpriteParser {
private:
    std::vector<uint8_t> imageData;
    BMPHeader header;
    int spriteWidth;
    int spriteHeight;
    int framesPerRow;
    int totalFrames;
    int bytesPerPixel;
    bool hasAlpha;
    
public:
    BMPSpriteParser(int sw, int sh) : spriteWidth(sw), spriteHeight(sh) {}
    
    bool loadBMP(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << filename << std::endl;
            return false;
        }
        
        // Read BMP header
        file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));
        
        // Validate BMP file
        if (header.signature != 0x4D42) { // 'BM' in little endian
            std::cerr << "Error: Not a valid BMP file" << std::endl;
            return false;
        }
        
        // Support both 24-bit and 32-bit BMP files
        if (header.bitsPerPixel != 24 && header.bitsPerPixel != 32) {
            std::cerr << "Error: Only 24-bit and 32-bit BMP files are supported" << std::endl;
            return false;
        }
        
        if (header.compression != 0) {
            std::cerr << "Error: Only uncompressed BMP files are supported" << std::endl;
            return false;
        }
        
        // Set bytes per pixel and alpha flag
        bytesPerPixel = header.bitsPerPixel / 8;
        hasAlpha = (header.bitsPerPixel == 32);
        
        std::cout << "BMP format: " << header.bitsPerPixel << "-bit " 
                  << (hasAlpha ? "(BGRA)" : "(BGR)") << std::endl;
        
        // Calculate sprite sheet dimensions
        int sheetWidth = header.width;
        int sheetHeight = abs(header.height);
        
        // Validate sprite sheet dimensions
        if (sheetWidth % spriteWidth != 0 || sheetHeight % spriteHeight != 0) {
            std::cerr << "Error: Sprite sheet dimensions (" << sheetWidth << "x" << sheetHeight 
                      << ") not compatible with sprite size (" << spriteWidth << "x" << spriteHeight << ")" << std::endl;
            return false;
        }
        
        framesPerRow = sheetWidth / spriteWidth;
        int framesPerColumn = sheetHeight / spriteHeight;
        totalFrames = framesPerRow * framesPerColumn;
        
        std::cout << "Sprite sheet: " << sheetWidth << "x" << sheetHeight << std::endl;
        std::cout << "Sprite size: " << spriteWidth << "x" << spriteHeight << std::endl;
        std::cout << "Frames per row: " << framesPerRow << std::endl;
        std::cout << "Total frames: " << totalFrames << std::endl;
        
        // Calculate row padding (BMP rows are padded to 4-byte boundaries)
        int rowPadding = (4 - (sheetWidth * bytesPerPixel) % 4) % 4;
        int paddedRowSize = sheetWidth * bytesPerPixel + rowPadding;
        
        // Read image data
        file.seekg(header.dataOffset);
        imageData.resize(paddedRowSize * sheetHeight);
        file.read(reinterpret_cast<char*>(imageData.data()), imageData.size());
        
        file.close();
        return true;
    }
    
    std::vector<uint8_t> extractSprite(int frameIndex, bool includeAlpha = false) {
        if (frameIndex >= totalFrames) {
            std::cerr << "Error: Frame index out of range" << std::endl;
            return {};
        }
        
        int frameCol = frameIndex % framesPerRow;
        int frameRow = frameIndex / framesPerRow;
        
        std::vector<uint8_t> spriteData;
        
        // Reserve space based on whether alpha is included
        int outputBytesPerPixel = includeAlpha && hasAlpha ? 4 : 3;
        spriteData.reserve(spriteWidth * spriteHeight * outputBytesPerPixel);
        
        int sheetWidth = header.width;
        int sheetHeight = abs(header.height);
        int rowPadding = (4 - (sheetWidth * bytesPerPixel) % 4) % 4;
        int paddedRowSize = sheetWidth * bytesPerPixel + rowPadding;
        
        // Extract sprite pixels
        for (int y = 0; y < spriteHeight; y++) {
            for (int x = 0; x < spriteWidth; x++) {
                // Calculate source position in sprite sheet
                int srcX = frameCol * spriteWidth + x;
                int srcY = frameRow * spriteHeight + y;
                
                // BMP is stored bottom-to-top, so flip Y coordinate
                if (header.height > 0) {
                    srcY = sheetHeight - 1 - srcY;
                }
                
                // Calculate byte offset in image data
                int offset = srcY * paddedRowSize + srcX * bytesPerPixel;
                
                // Read pixel data based on format
                uint8_t b = imageData[offset];
                uint8_t g = imageData[offset + 1];
                uint8_t r = imageData[offset + 2];
                uint8_t a = hasAlpha ? imageData[offset + 3] : 255;
                
                // Store as RGB or RGBA
                spriteData.push_back(r);
                spriteData.push_back(g);
                spriteData.push_back(b);
                
                if (includeAlpha && hasAlpha) {
                    spriteData.push_back(a);
                }
            }
        }
        
        return spriteData;
    }
    
    void generateProgmemArray(const std::string& arrayName, bool includeAlpha = false, const std::string& outputFile = "") {
        std::ostream* out = &std::cout;
        std::ofstream file;
        
        if (!outputFile.empty()) {
            file.open(outputFile);
            if (file.is_open()) {
                out = &file;
            }
        }
        
        int outputBytesPerPixel = includeAlpha && hasAlpha ? 4 : 3;
        std::string format = includeAlpha && hasAlpha ? "RGBA8888 (32-bit)" : "RGB888 (24-bit)";
        
        *out << "// Generated sprite array from " << header.bitsPerPixel << "-bit BMP sprite sheet\n";
        *out << "// Sprite dimensions: " << spriteWidth << "x" << spriteHeight << std::endl;
        *out << "// Total frames: " << totalFrames << std::endl;
        *out << "// Output format: " << format << std::endl;
        *out << "// Alpha channel: " << (includeAlpha && hasAlpha ? "included" : "discarded") << "\n\n";
        
        *out << "#include <pgmspace.h>\n\n";
        
        // Generate individual frame arrays
        for (int frame = 0; frame < totalFrames; frame++) {
            std::vector<uint8_t> spriteData = extractSprite(frame, includeAlpha);
            
            *out << "const uint8_t PROGMEM " << arrayName << "_frame" << frame << "[] = {\n";
            
            for (size_t i = 0; i < spriteData.size(); i += outputBytesPerPixel) {
                if (i % (spriteWidth * outputBytesPerPixel) == 0 && i > 0) {
                    *out << "\n";
                }
                
                *out << "  ";
                for (int j = 0; j < outputBytesPerPixel; j++) {
                    *out << "0x" << std::hex << std::setfill('0') << std::setw(2) 
                         << static_cast<int>(spriteData[i + j]);
                    if (j < outputBytesPerPixel - 1) {
                        *out << ", ";
                    }
                }
                
                if (i + outputBytesPerPixel < spriteData.size()) {
                    *out << ",";
                }
                
                if ((i / outputBytesPerPixel + 1) % spriteWidth == 0) {
                    *out << std::dec;
                }
            }
            
            *out << std::dec << "\n};\n\n";
        }
        
        // Generate array of pointers for easy animation access
        *out << "const uint8_t* const PROGMEM " << arrayName << "_frames[] = {\n";
        for (int frame = 0; frame < totalFrames; frame++) {
            *out << "  " << arrayName << "_frame" << frame;
            if (frame < totalFrames - 1) {
                *out << ",";
            }
            *out << "\n";
        }
        *out << "};\n\n";
        
        // Generate constants
        std::string baseName = arrayName.substr(0, arrayName.find_last_not_of("0123456789") + 1);
        *out << "const int " << baseName << "FRAME_COUNT = " << totalFrames << ";\n";
        *out << "const int " << baseName << "SPRITE_WIDTH = " << spriteWidth << ";\n";
        *out << "const int " << baseName << "SPRITE_HEIGHT = " << spriteHeight << ";\n";
        *out << "const int " << baseName << "BYTES_PER_PIXEL = " << outputBytesPerPixel << ";\n";
        *out << "const bool " << baseName << "HAS_ALPHA = " << (includeAlpha && hasAlpha ? "true" : "false") << ";\n";
        
        if (file.is_open()) {
            file.close();
            std::cout << "PROGMEM array generated in " << outputFile << std::endl;
        }
    }
    
    int getFrameCount() const { return totalFrames; }
    bool getHasAlpha() const { return hasAlpha; }
    int getBitsPerPixel() const { return header.bitsPerPixel; }
};

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " <bmp_file> <sprite_width> <sprite_height> [array_name] [include_alpha] [output_file]" << std::endl;
        std::cout << "Example: " << argv[0] << " fire_sprites.bmp 8 8 fireSprites true sprites.h" << std::endl;
        std::cout << "include_alpha: true/false (default: false - alpha channel discarded)" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    int spriteWidth = std::stoi(argv[2]);
    int spriteHeight = std::stoi(argv[3]);
    std::string arrayName = (argc > 4) ? argv[4] : "sprite";
    bool includeAlpha = (argc > 5) ? (std::string(argv[5]) == "true") : false;
    std::string outputFile = (argc > 6) ? argv[6] : "";
    
    BMPSpriteParser parser(spriteWidth, spriteHeight);
    
    if (!parser.loadBMP(filename)) {
        return 1;
    }
    
    std::cout << "Alpha channel support: " << (parser.getHasAlpha() ? "Available" : "Not available") << std::endl;
    std::cout << "Including alpha in output: " << (includeAlpha && parser.getHasAlpha() ? "Yes" : "No") << std::endl;
    
    parser.generateProgmemArray(arrayName, includeAlpha, outputFile);
    
    std::cout << "Successfully parsed " << parser.getFrameCount() 
              << " frames from " << parser.getBitsPerPixel() << "-bit sprite sheet!" << std::endl;
    
    return 0;
}
