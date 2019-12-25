#pragma once

#include "SpiCommands.h"
#include "fonts/fonts.h"

struct Image {
    uint16_t imageName;  // max = 128K / (imageWidth/8 * imageHeight)
    uint16_t imageOffset;
    uint16_t imageWidth;
    uint16_t imageHeight;
    uint16_t imageRotate;
    uint16_t imageColor;
    uint16_t memoryWidth;
    uint16_t memoryHeight;
    uint16_t mirror;
};

enum dot_pixel {
    dot_pixel_1x1 = 1,  // 1 x 1
    dot_pixel_2x2,      // 2 X 2
    dot_pixel_3x3,      // 3 X 3
    dot_pixel_4x4,      // 4 X 4
    dot_pixel_5x5,      // 5 X 5
    dot_pixel_6x6,      // 6 X 6
    dot_pixel_7x7,      // 7 X 7
    dot_pixel_8x8,      // 8 X 8
};

enum dot_style {
    dot_fill_around = 1,  // dot pixel 1 x 1
    dot_fill_rightup,     // dot pixel 2 X 2
};

enum line_style {
    line_style_solid = 0,
    line_style_dotted,
};

enum draw_fill {
    draw_fill_empty = 0,
    draw_fill_full,
};

enum mirror_image {
    mirror_none = 0x00,
    mirror_horizontal = 0x01,
    mirror_vertical = 0x02,
    mirror_origin = 0x03,
};

enum angle {
    angle_0 = 0,
    angle_90,
    angle_180,
    angle_270,
};

enum color_state {
    color_positive = 0X01,
    color_inverted = 0X02,
};

enum image_color {
    black_image = 0,
    red_image = 1,
    yellow_image = 1,
};

enum color {
    white = 0xFF,
    black = 0x00,
    red = 0x00,
    yellow = 0x00,
};

enum background_color {
    image_background = 0xFF,
    font_foreground = 0x00,
    font_background = 0xFF,
};

class Display {
   private:
    uint8_t _rotate = angle_0;
    dot_style _dot_style = dot_fill_around;
    dot_pixel _dot_pixel = dot_pixel_1x1;
    line_style _line_style = line_style_solid;
    draw_fill _draw_fill = draw_fill_empty;
    uint16_t _colorBackground = white;
    Sfont *_Font = &sFont20;

    void swop(uint16_t point1, uint16_t point2);
    void drawPixel(uint16_t xPoint, uint16_t yPoint, uint16_t color);
    void setPixel(uint16_t xPoint, uint16_t yPoint, uint16_t color);
    void drawChar(uint16_t xStart, uint16_t yStart, const char acsiiChar, Sfont *font, uint16_t colorBackground, uint16_t colorForeground);

   protected:
    SpiCommands mySPI;
    uint8_t _color = black;
    uint16_t _width;
    uint16_t _height;

   public:
    Image image;
    Display(uint16_t width, uint16_t height);

    ~Display() {}
    void setColor(uint8_t color) { _color = color; }
    void setRotate(uint8_t rotate) { _rotate = rotate; }
    void setDotStyle(dot_style dot_style) { _dot_style = dot_style; }
    void setDotPixel(dot_pixel dot_pixel) { _dot_pixel = dot_pixel; }
    void setLineStyle(line_style line_style) { _line_style = line_style; }
    void setDrawFill(draw_fill draw_fill) { _draw_fill = draw_fill; }
    void setFont(Sfont *Font) { _Font = Font; }

    void newImage(uint16_t imageName);
    void clearWindow(uint16_t color);

    void drawPoint(uint16_t xPoint, uint16_t yPoint);
    void drawLine(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
    void drawRectangle(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
    void drawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t radius);

    void print(uint16_t xStart, uint16_t yStart, const char *pString);
    void print(uint16_t xStart, uint16_t yStart, int32_t num);
    void print(uint16_t xStart, uint16_t yStart, String s);

    void drawBitMap(const unsigned char *imageBuffer);
    
    void reset(void);
    void sendCommand(uint8_t reg);
    void sendData(uint8_t data);
    void readBusy(void);
};
