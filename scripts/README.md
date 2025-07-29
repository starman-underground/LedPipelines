# ConvertFontToProgmem.cpp
This script converts a TrueType font file into a C array compressed font atlas along with glyph metadata for a printable subset of ASCII characters.
- We want to display text on our LED matrix, but microcontroller do not have enough memory/compute to store/process entire TrueType font file.
- So, we must extract only the bitmaps for the characters we need for the dimensions we want them in.
    - The printable ASCII characters make a good starting subset.
        ```cpp
        constexpr int FIRST_CHAR = 32;
        constexpr int LAST_CHAR = 126;
        ```
    - The proper max character height/width depends on your display resolution.
        - Specifying height is enough as the font allows the relative width to be inferred.
- In addition to storing character bitmaps (glyphs), we must also store glyph metadata to read and render each character.
    ```cpp
    struct GlyphMetrics { // Glyph = visual representation of character
        uint8_t width;
        uint8_t height;
        int8_t xOffset; // Horizontal offset from cursor to left side of glyph
        int8_t yOffset; // Vertical offset from bottom line to top side of glyph
        uint8_t advanceWidth; // How many pixels to advance the cursor
        uint16_t dataOffset; // Offset in bits from start of bitmap data
    };
    ```
- This data is then written to the header file `../include/resources/FontAtlas.h` which is linked to by `LedPipelineUtils.h`.
## Building
```bash
clang++ -std=c++17 -O2 ConvertFontToProgmem.cpp -o ConvertFontToProgmem.exe
```
## Use
```bash
.\ConvertFontToProgmem.exe <font.ttf> <targetHeight>
```
- The output from the script is written to `../include/resources/FontAtlas.h`.
- The font must be a TrueType font file (.ttf).
- The target height is the maximum height in pixels of a character in the generated font.
- Here is an example used to generate the character atlas used in example 004 (fonts\TeenyTinyPixls-o2zo.ttf is not included due to licensing but can be found at https://resourceboy.com/fonts/teeny-tiny-pixls-font/).
    ```bash
    .\ConvertFontToProgmem.exe .\fonts\TeenyTinyPixls-o2zo.ttf 8
    ```
# ConvertBmpToProgmem.cpp
This script converts a Bitmap image file into 1 or more C arrays for storage of small static images and animation frames.
- If you use the script to convert a single image file into multiple C arrays with each array representing a frame, each frame must be the same size and stored horizontally in the bitmap image.
- Once you use the script to generate image data, it may be used in the following manner to create a looping fire animation:
    ```cpp
    auto fireEffect = new LoopEffect(
        (new SeriesLedPipeline())
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame0, TwoDimensionalLayout::VERTICAL_SNAKE_XYFLIP, 8, 8, 50), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame1, TwoDimensionalLayout::VERTICAL_SNAKE_XYFLIP, 8, 8, 50), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame2, TwoDimensionalLayout::VERTICAL_SNAKE_XYFLIP, 8, 8, 50), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame3, TwoDimensionalLayout::VERTICAL_SNAKE_XYFLIP, 8, 8, 50), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame4, TwoDimensionalLayout::VERTICAL_SNAKE_XYFLIP, 8, 8, 50), 0.05))
            ->addStage(new TimeBoxedEffect(new ImageEffect(fireAnimation_frame5, TwoDimensionalLayout::VERTICAL_SNAKE_XYFLIP, 8, 8, 50), 0.05))
    );
    ```
## Building
```bash
clang++ -std=c++17 -O2 ConvertBmpToProgmem.cpp -o ConvertBmpToProgmem.exe
```
## Use
```bash
.\ConvertBmpToProgmem.exe <image.bmp> <sprite_width> <sprite_height> [array_name] [include_alpha] [output_header_file]
```
- The pixel dimensions of the image file must be a multiple of the sprite dimensions (i.e. image_width % sprite_width = image_height % sprite_height = 0).
- The array_name argument determines the name of the C arrays in the generated header file.
- Pass 'true' as the 5th cmdline arg if your image has an alpha channel.
- The order of sprite frames starts at the top left of the image and proceeds left to right and then top to bottom.
- See include/resources/FireSprites.h for an example of the format of generated data
- Here is an example that was used to generate the sprites for the fire animation in example 003 using the spritesheet at `scripts/sprites/fire_anim_spritesheet.bmp`:
    ```bash
    .\ConvertBmpToProgmem.exe .\sprites\fire_anim_spritesheet.bmp 8 8 fireAnimation true FireSprites.h
    ```