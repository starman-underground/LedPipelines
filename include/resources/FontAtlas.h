#pragma once
#include <Arduino.h>
#include <stdint.h>

// Optimized Variable-Width Font Atlas Configuration
constexpr int FIRST_ASCII = 32;
constexpr int LAST_ASCII = 126;
constexpr int NUM_CHARACTERS = 95;
constexpr int FONT_MAX_HEIGHT = 7;

struct GlyphMetrics {
    uint8_t width;
    uint8_t height;
    int8_t xOffset;
    int8_t yOffset;
    uint8_t advanceWidth;
    uint16_t dataOffset; // Offset in bits
};

const GlyphMetrics GLYPH_METRICS[] PROGMEM = {
    {1, 1, 0, 0, 2, 0}, // ' '
    {2, 7, 1, -7, 5, 1}, // '!'
    {4, 4, 0, -7, 5, 15}, // '"'
    {4, 7, 0, -7, 5, 31}, // '#'
    {4, 7, 0, -7, 5, 59}, // '$'
    {4, 7, 0, -7, 5, 87}, // '%'
    {4, 7, 0, -7, 5, 115}, // '&'
    {2, 4, 1, -7, 3, 143}, // '''
    {3, 7, 0, -7, 5, 151}, // '('
    {3, 7, 0, -7, 5, 172}, // ')'
    {4, 6, 0, -7, 5, 193}, // '*'
    {4, 5, 0, -6, 5, 217}, // '+'
    {2, 3, 1, -3, 3, 237}, // ','
    {4, 2, 0, -4, 5, 243}, // '-'
    {2, 2, 1, -2, 3, 251}, // '.'
    {4, 7, 0, -7, 5, 255}, // '/'
    {4, 7, 0, -7, 5, 283}, // '0'
    {4, 7, 0, -7, 5, 311}, // '1'
    {4, 7, 0, -7, 5, 339}, // '2'
    {4, 7, 0, -7, 5, 367}, // '3'
    {4, 7, 0, -7, 5, 395}, // '4'
    {4, 7, 0, -7, 5, 423}, // '5'
    {4, 7, 0, -7, 5, 451}, // '6'
    {4, 7, 0, -7, 5, 479}, // '7'
    {4, 7, 0, -7, 5, 507}, // '8'
    {4, 7, 0, -7, 5, 535}, // '9'
    {2, 5, 1, -6, 3, 563}, // ':'
    {2, 6, 1, -6, 3, 573}, // ';'
    {4, 7, 0, -7, 5, 585}, // '<'
    {4, 5, 0, -6, 5, 613}, // '='
    {4, 7, 0, -7, 5, 633}, // '>'
    {4, 7, 0, -7, 5, 661}, // '?'
    {4, 7, 0, -7, 5, 689}, // '@'
    {4, 7, 0, -7, 5, 717}, // 'A'
    {4, 7, 0, -7, 5, 745}, // 'B'
    {4, 7, 0, -7, 5, 773}, // 'C'
    {4, 7, 0, -7, 5, 801}, // 'D'
    {4, 7, 0, -7, 5, 829}, // 'E'
    {4, 7, 0, -7, 5, 857}, // 'F'
    {4, 7, 0, -7, 5, 885}, // 'G'
    {4, 7, 0, -7, 5, 913}, // 'H'
    {4, 7, 0, -7, 5, 941}, // 'I'
    {4, 7, 0, -7, 5, 969}, // 'J'
    {4, 7, 0, -7, 5, 997}, // 'K'
    {4, 7, 0, -7, 5, 1025}, // 'L'
    {4, 7, 0, -7, 5, 1053}, // 'M'
    {4, 7, 0, -7, 5, 1081}, // 'N'
    {4, 7, 0, -7, 5, 1109}, // 'O'
    {4, 7, 0, -7, 5, 1137}, // 'P'
    {4, 7, 0, -7, 5, 1165}, // 'Q'
    {4, 7, 0, -7, 5, 1193}, // 'R'
    {4, 7, 0, -7, 5, 1221}, // 'S'
    {4, 7, 0, -7, 5, 1249}, // 'T'
    {4, 7, 0, -7, 5, 1277}, // 'U'
    {4, 7, 0, -7, 5, 1305}, // 'V'
    {4, 7, 0, -7, 5, 1333}, // 'W'
    {4, 7, 0, -7, 5, 1361}, // 'X'
    {4, 7, 0, -7, 5, 1389}, // 'Y'
    {4, 7, 0, -7, 5, 1417}, // 'Z'
    {3, 7, 0, -7, 4, 1445}, // '['
    {4, 7, 0, -7, 5, 1466}, // '\'
    {3, 7, 0, -7, 4, 1494}, // ']'
    {4, 4, 0, -7, 5, 1515}, // '^'
    {4, 2, 0, -2, 5, 1531}, // '_'
    {3, 4, 0, -7, 4, 1539}, // '`'
    {4, 6, 0, -6, 5, 1551}, // 'a'
    {4, 7, 0, -7, 5, 1575}, // 'b'
    {4, 6, 0, -6, 5, 1603}, // 'c'
    {4, 7, 0, -7, 5, 1627}, // 'd'
    {4, 6, 0, -6, 5, 1655}, // 'e'
    {4, 7, 0, -7, 5, 1679}, // 'f'
    {4, 6, 0, -6, 5, 1707}, // 'g'
    {4, 7, 0, -7, 5, 1731}, // 'h'
    {4, 7, 0, -7, 5, 1759}, // 'i'
    {3, 7, 0, -7, 5, 1787}, // 'j'
    {4, 7, 0, -7, 5, 1808}, // 'k'
    {4, 7, 0, -7, 5, 1836}, // 'l'
    {4, 6, 0, -6, 5, 1864}, // 'm'
    {4, 6, 0, -6, 5, 1888}, // 'n'
    {4, 6, 0, -6, 5, 1912}, // 'o'
    {4, 6, 0, -6, 5, 1936}, // 'p'
    {4, 6, 0, -6, 5, 1960}, // 'q'
    {4, 6, 0, -6, 5, 1984}, // 'r'
    {4, 6, 0, -6, 5, 2008}, // 's'
    {4, 6, 0, -6, 5, 2032}, // 't'
    {4, 6, 0, -6, 5, 2056}, // 'u'
    {4, 6, 0, -6, 5, 2080}, // 'v'
    {4, 6, 0, -6, 5, 2104}, // 'w'
    {4, 6, 0, -6, 5, 2128}, // 'x'
    {4, 6, 0, -6, 5, 2152}, // 'y'
    {4, 6, 0, -6, 5, 2176}, // 'z'
    {4, 7, 0, -7, 5, 2200}, // '{'
    {2, 7, 1, -7, 3, 2228}, // '|'
    {4, 7, 0, -7, 5, 2242}, // '}'
    {4, 5, 0, -6, 5, 2270} // '~'
};

const uint8_t COMPRESSED_FONT_DATA[] PROGMEM = {
    0x1f,0x87,0x33,0x21,0x3f,0xf3,0xff,0x26,0xf0,0xc2,0x3d,0x32,0x2d,0x11,0x39,0x91,
    0xf9,0x9e,0x78,0x14,0x92,0x39,0x1b,0x6a,0x03,0x7b,0x4c,0x83,0x7b,0x01,0xfe,0x06,
    0x22,0xcd,0x11,0x1f,0xf3,0x33,0xff,0x9c,0xcc,0xcd,0xff,0xe3,0xf9,0x9f,0xfe,0x3e,
    0x67,0xf3,0x33,0xe6,0x23,0xff,0x1e,0x67,0xff,0xf1,0xff,0xff,0xff,0x22,0x22,0x3f,
    0xf3,0xff,0xff,0xff,0x3e,0x62,0x26,0x01,0x87,0x88,0xb4,0x21,0x08,0x78,0x7f,0xc4,
    0x30,0x92,0x47,0xf8,0xb8,0x03,0x7f,0xcc,0xc6,0x7f,0xfc,0xff,0xcc,0xff,0xcf,0x6f,
    0xff,0xfc,0x44,0x67,0xe7,0x4c,0xce,0xf7,0xfc,0x7e,0x67,0xff,0xc7,0xe4,0x47,0xfc,
    0x4c,0xff,0xcc,0xcf,0xfc,0xcf,0xfb,0x33,0x37,0x88,0x88,0xcf,0xfc,0xcc,0xf6,0xcc,
    0xc4,0x44,0x46,0x7c,0xff,0xcc,0xcc,0xff,0xcc,0xcc,0xcf,0xfc,0xcc,0xff,0xff,0xcf,
    0xe4,0x47,0xfc,0xcf,0x74,0xff,0xcf,0x6c,0xcf,0xfc,0x79,0x9f,0xff,0xb3,0x33,0x34,
    0xcc,0xcc,0xff,0xcc,0xcc,0xcc,0xb4,0xcc,0xcf,0xfc,0xcc,0xcb,0x4c,0xcc,0xcc,0xfb,
    0x33,0x7f,0x8b,0x46,0x7e,0xd2,0x4d,0xe2,0x19,0x84,0x47,0x76,0xdb,0xe0,0xd2,0x01,
    0xf2,0x30,0x0f,0x33,0x6f,0x19,0xf3,0x3f,0xe1,0xf1,0x19,0xe2,0x7f,0x33,0xfe,0x0f,
    0x3d,0xce,0x22,0xde,0xcc,0xc1,0xd2,0xef,0xd1,0x9f,0x33,0x32,0x00,0x0c,0xcd,0xe0,
    0x06,0xdf,0x88,0x9e,0xd9,0x9c,0xe6,0x66,0x67,0x09,0xff,0x99,0x0f,0x99,0x99,0x0f,
    0x99,0xff,0x0f,0x9f,0xf8,0x0f,0x9f,0xf1,0x0f,0x88,0x88,0x07,0xe7,0x7e,0x06,0xf6,
    0x67,0x09,0x99,0xff,0x09,0x99,0x96,0x09,0x9f,0xf9,0x09,0x69,0x99,0x09,0x9f,0xf1,
    0x0f,0x78,0xcf,0x37,0x6e,0x66,0x73,0xff,0xf3,0x99,0xd9,0xb8,0x07,0xf2,0x00
};

// Helper function to get glyph metrics (undefined behavior if char is out of range: [' ' (20), '~' (126)])
inline const GlyphMetrics& getGlyphMetrics(char c) {
    return GLYPH_METRICS[c - FIRST_ASCII];
}

// Helper function to extract glyph bitmap
// Returns true if pixel at (x,y) is set, false otherwise
inline bool getGlyphPixel(char c, int x, int y) {
    const GlyphMetrics& metrics = getGlyphMetrics(c);
    if (x >= metrics.width || y >= metrics.height || x < 0 || y < 0) {
        return false;
    }
    
    int bitOffset = metrics.dataOffset + (y * metrics.width + x);
    int byteIndex = bitOffset / 8;
    int bitPosition = 7 - (bitOffset % 8);
    
    return (COMPRESSED_FONT_DATA[byteIndex] & (1 << bitPosition)) != 0;
}
