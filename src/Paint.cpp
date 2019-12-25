#include "Paint.h"
#include <stdint.h>

void Paint ::swop(uint16_t point1, uint16_t point2) {
    uint16_t temp;
    temp = point1;
    point1 = point2;
    point2 = temp;
}

Paint ::Paint(uint16_t width, uint16_t height) {
    _width = width;
    _height = height;
    image.imageHeight = height;
    image.imageWidth = width;
    mySPI.init();
}

void Paint ::newImage(uint16_t imageName) {
    uint16_t width = _width;
    uint16_t height = _height;
    uint16_t color = _color;
    uint8_t rotate = _rotate;
    mySPI.setMode(BYTE_MODE);

    if (rotate == angle_0 || rotate == angle_180) {
        image.imageWidth = width;
        image.imageHeight = height;
    } else {
        image.imageWidth = height;
        image.imageHeight = width;
    }
    image.imageName = imageName;  // At least one picture
    image.imageRotate = rotate;
    image.memoryWidth = width;
    image.memoryHeight = height;
    image.imageColor = color;

    image.mirror = mirror_none;

    uint16_t BYTE_Height = image.memoryHeight;
    uint16_t BYTE_Width = (image.memoryWidth % 8 == 0)
                              ? (image.memoryWidth / 8)
                              : (image.memoryWidth / 8 + 1);
    image.imageOffset = image.imageName * (BYTE_Height * BYTE_Width);
}

void Paint ::drawPixel(uint16_t xPoint, uint16_t yPoint, uint16_t color) {
    // uint16_t height =  image.memoryHeight;
    uint16_t width = (image.memoryWidth % 8 == 0) ? (image.memoryWidth / 8)
                                                  : (image.memoryWidth / 8 + 1);
    uint32_t offset = image.imageOffset;

    switch (image.mirror) {
        case mirror_none:
            break;
        case mirror_horizontal:
            xPoint = image.memoryWidth - xPoint - 1;
            break;
        case mirror_vertical:
            yPoint = image.memoryHeight - yPoint - 1;
            break;
        case mirror_origin:
            xPoint = image.memoryWidth - xPoint - 1;
            yPoint = image.memoryHeight - yPoint - 1;
            break;
        default:
            return;
    }

    uint32_t addr = xPoint / 8 + yPoint * width + offset;
    uint8_t rData = mySPI.readByte(addr);

    if (image.imageColor == color_positive) {
        if (color == white)
            mySPI.writeByte(addr, rData & ~(0x80 >> (xPoint % 8)));
        else
            mySPI.writeByte(addr, rData | (0x80 >> (xPoint % 8)));
    } else {
        if (color == black)
            mySPI.writeByte(addr, rData & ~(0x80 >> (xPoint % 8)));
        else
            mySPI.writeByte(addr, rData | (0x80 >> (xPoint % 8)));
    }
}

void Paint ::clearWindow(uint16_t color) {
    uint16_t height = image.memoryHeight;
    uint16_t width = (image.memoryWidth % 8 == 0) ? (image.memoryWidth / 8)
                                                  : (image.memoryWidth / 8 + 1);
    uint32_t offset = image.imageOffset;

    if (image.imageColor == color_inverted) {
        color = ~color;
    }
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {  // 8 pixel =  1 byte
            mySPI.writeByte((x + y * width) + offset, color);
        }
    }
}

void Paint ::setPixel(uint16_t xPoint, uint16_t yPoint, uint16_t color) {
    uint16_t x, y;
    switch (image.imageRotate) {
        case angle_0:
            x = xPoint;
            y = yPoint;
            drawPixel(x, y, color);
            break;
        case angle_90:
            x = yPoint;
            y = image.imageWidth - xPoint;
            drawPixel(x, y, color);
            break;
        case angle_180:
            x = image.imageWidth - xPoint;
            y = image.imageHeight - yPoint;
            drawPixel(x, y, color);
            break;
        case angle_270:
            x = image.imageHeight - yPoint;
            y = xPoint;
            drawPixel(x, y, color);
            break;
    }
}

void Paint ::drawPoint(uint16_t xPoint, uint16_t yPoint) {
    uint16_t color = _color;
    dot_style dot_style = _dot_style;
    dot_pixel dot_pixel = _dot_pixel;
    if (xPoint > image.imageWidth || yPoint > image.imageHeight) {
        Serial.print(" drawPoint Input exceeds the normal display range\r\n");
        return;
    }

    int16_t xDirNum, yDirNum;
    if (dot_style == dot_fill_around) {
        for (xDirNum = 0; xDirNum < 2 * dot_pixel - 1; xDirNum++) {
            for (yDirNum = 0; yDirNum < 2 * dot_pixel - 1; yDirNum++) {
                setPixel(xPoint + xDirNum - dot_pixel, yPoint + yDirNum - dot_pixel,
                         color);
            }
        }
    } else {
        for (xDirNum = 0; xDirNum < dot_pixel; xDirNum++) {
            for (yDirNum = 0; yDirNum < dot_pixel; yDirNum++) {
                setPixel(xPoint + xDirNum - 1, yPoint + yDirNum - 1, color);
            }
        }
    }
}

void Paint ::drawLine(uint16_t xStart, uint16_t yStart, uint16_t xEnd,
                      uint16_t yEnd) {
    uint16_t color = _color;
    line_style line_style = _line_style;
    dot_pixel dot_pixel = _dot_pixel;

    if (xStart > image.imageWidth || yStart > image.imageHeight ||
        xEnd > image.imageWidth || yEnd > image.imageHeight) {
        Serial.print(" drawLine Input exceeds the normal display range\r\n");
        return;
    }

    if (xStart > xEnd)
        swop(xStart, xEnd);
    if (yStart > yEnd)
        swop(yStart, yEnd);

    uint16_t xPoint = xStart;
    uint16_t yPoint = yStart;
    int dx = (int)xEnd - (int)xStart >= 0 ? xEnd - xStart : xStart - xEnd;
    int dy = (int)yEnd - (int)yStart <= 0 ? yEnd - yStart : yStart - yEnd;

    // Increment direction, 1 is positive, -1 is counter;
    int xAddWay = xStart < xEnd ? 1 : -1;
    int yAddWay = yStart < yEnd ? 1 : -1;

    // Cumulative error
    int esp = dx + dy;
    char dottedLen = 0;

    for (;;) {
        dottedLen++;
        // Painted dotted line, 2 point is really virtual
        if (line_style == line_style_dotted && dottedLen % 3 == 0) {
            // Serial.print("LINE_DOTTED\r\n");
            setDotStyle(dot_fill_around);
            setDotPixel(dot_pixel);
            setColor(image_background);
            drawPoint(xPoint, yPoint);
            dottedLen = 0;
        } else {
            setDotStyle(dot_fill_around);
            setDotPixel(dot_pixel);
            setColor(color);
            drawPoint(xPoint, yPoint);
        }
        if (2 * esp >= dy) {
            if (xPoint == xEnd)
                break;
            esp += dy;
            xPoint += xAddWay;
        }
        if (2 * esp <= dx) {
            if (yPoint == yEnd)
                break;
            esp += dx;
            yPoint += yAddWay;
        }
    }
}

void Paint ::drawRectangle(uint16_t xStart, uint16_t yStart, uint16_t xEnd,
                           uint16_t yEnd) {
    uint16_t color = _color;
    draw_fill draw_fill = _draw_fill;
    dot_pixel dot_pixel = _dot_pixel;
    if (xStart > image.imageWidth || yStart > image.imageHeight ||
        xEnd > image.imageWidth || yEnd > image.imageHeight) {
        Serial.print("Input exceeds the normal display range\r\n");
        return;
    }

    if (xStart > xEnd)
        swop(xStart, xEnd);
    if (yStart > yEnd)
        swop(yStart, yEnd);

    if (draw_fill) {
        uint16_t yPoint;
        for (yPoint = yStart; yPoint < yEnd; yPoint++) {
            setColor(color);
            setDotPixel(dot_pixel);
            setLineStyle(line_style_solid);
            drawLine(xStart, yPoint, xEnd, yPoint);
        }
    } else {
        setColor(color);
        setDotPixel(dot_pixel);
        setLineStyle(line_style_solid);
        drawLine(xStart, yStart, xEnd, yStart);
        drawLine(xStart, yStart, xStart, yEnd);
        drawLine(xEnd, yEnd, xEnd, yStart);
        drawLine(xEnd, yEnd, xStart, yEnd);
    }
}

void Paint ::drawCircle(uint16_t xCenter, uint16_t yCenter, uint16_t radius) {
    uint16_t color = _color;
    draw_fill draw_fill = _draw_fill;
    dot_pixel dot_pixel = _dot_pixel;
    if (xCenter > image.imageWidth || yCenter >= image.imageHeight) {
        Serial.print(" drawCircle Input exceeds the normal display range\r\n");
        return;
    }

    // Draw a circle from(0, R) as a starting point
    int16_t xCurrent, yCurrent;
    xCurrent = 0;
    yCurrent = radius;

    // Cumulative error,judge the next point of the logo
    int16_t esp = 3 - (radius << 1);

    int16_t sCountY;
    if (draw_fill == draw_fill_full) {
        while (xCurrent <= yCurrent) {  // Realistic circles
            for (sCountY = xCurrent; sCountY <= yCurrent; sCountY++) {
                setDotStyle(dot_fill_around);
                setDotPixel(dot_pixel_1x1);
                setColor(color);
                drawPoint(xCenter + xCurrent, yCenter + sCountY);  // 1
                drawPoint(xCenter - xCurrent, yCenter + sCountY);  // 2
                drawPoint(xCenter - sCountY, yCenter + xCurrent);  // 3
                drawPoint(xCenter - sCountY, yCenter - xCurrent);  // 4
                drawPoint(xCenter - xCurrent, yCenter - sCountY);  // 5
                drawPoint(xCenter + xCurrent, yCenter - sCountY);  // 6
                drawPoint(xCenter + sCountY, yCenter - xCurrent);  // 7
                drawPoint(xCenter + sCountY, yCenter + xCurrent);
            }
            if (esp < 0)
                esp += 4 * xCurrent + 6;
            else {
                esp += 10 + 4 * (xCurrent - yCurrent);
                yCurrent--;
            }
            xCurrent++;
        }
    } else {  // Draw a hollow circle
        while (xCurrent <= yCurrent) {
            setDotStyle(dot_fill_around);
            setDotPixel(dot_pixel);
            setColor(color);
            drawPoint(xCenter + xCurrent, yCenter + yCurrent);  // 1
            drawPoint(xCenter - xCurrent, yCenter + yCurrent);  // 2
            drawPoint(xCenter - yCurrent, yCenter + xCurrent);  // 3
            drawPoint(xCenter - yCurrent, yCenter - xCurrent);  // 4
            drawPoint(xCenter - xCurrent, yCenter - yCurrent);  // 5
            drawPoint(xCenter + xCurrent, yCenter - yCurrent);  // 6
            drawPoint(xCenter + yCurrent, yCenter - xCurrent);  // 7
            drawPoint(xCenter + yCurrent, yCenter + xCurrent);  // 0

            if (esp < 0)
                esp += 4 * xCurrent + 6;
            else {
                esp += 10 + 4 * (xCurrent - yCurrent);
                yCurrent--;
            }
            xCurrent++;
        }
    }
}

void Paint ::drawChar(uint16_t xPoint, uint16_t yPoint, const char asciiChar,
                      Sfont *Font, uint16_t colorBackground,
                      uint16_t colorForeground) {
    uint16_t page, column;

    if (xPoint > image.imageWidth || yPoint > image.imageHeight) {
        Serial.print(" drawChar Input exceeds the normal display range\r\n");
        return;
    }

    uint32_t charOffset = (asciiChar - ' ') * Font->height *
                          (Font->width / 8 + (Font->width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[charOffset];

    for (page = 0; page < Font->height; page++) {
        for (column = 0; column < Font->width; column++) {
            // To determine whether the font background color and screen background
            // color is consistent
            if (font_background ==
                colorBackground) {  // this process is to speed up the scan
                if (pgm_read_byte(ptr) & (0x80 >> (column % 8))) {
                    setDotStyle(dot_fill_around);
                    setDotPixel(dot_pixel_1x1);
                    setColor(colorForeground);
                    drawPoint(xPoint + column, yPoint + page);
                }
            } else {
                setDotStyle(dot_fill_around);
                setDotPixel(dot_pixel_1x1);
                if (pgm_read_byte(ptr) & (0x80 >> (column % 8))) {
                    setColor(colorForeground);
                    drawPoint(xPoint + column, yPoint + page);
                } else {
                    setColor(colorBackground);
                    drawPoint(xPoint + column, yPoint + page);
                }
            }
            // One pixel is 8 bits
            if (column % 8 == 7)
                ptr++;
        }  // Write a line
        if (Font->width % 8 != 0)
            ptr++;
    }  // Write all
}

void Paint ::print(uint16_t xStart, uint16_t yStart, const char *pString) {
    uint16_t colorBackground = _colorBackground;
    uint16_t colorForeground = _color;
    Sfont *Font = _Font;

    uint16_t xPoint = xStart;
    uint16_t yPoint = yStart;

    if (xStart > image.imageWidth || yStart > image.imageHeight) {
        Serial.print(" drawString Input exceeds the normal display range\r\n");
        return;
    }

    while (*pString != '\0') {
        // if X direction filled , reposition to(xStart,yPoint),yPoint is y
        // direction plus the height of the character
        if ((xPoint + Font->width) > image.imageWidth) {
            xPoint = xStart;
            yPoint += Font->height;
        }

        // If the y direction is full, reposition to(xStart, yStart)
        if ((yPoint + Font->height) > image.imageHeight) {
            xPoint = xStart;
            yPoint = yStart;
        }
        drawChar(xPoint, yPoint, *pString, Font, colorBackground, colorForeground);

        // The next character of the address
        pString++;

        // The next word of the abscissa increases the font of the broadband
        xPoint += Font->width;
    }
}

void Paint ::print(uint16_t xStart, uint16_t yStart, int32_t num) {
    String str = String(num);
    print(xStart, yStart, str.c_str());
}

void Paint ::print(uint16_t xStart, uint16_t yStart, String s) {
    print(xStart, yStart, s.c_str());
}

void Paint ::drawBitMap(const unsigned char *imageBuffer) {
    uint16_t xPoint, yPoint, height, width;
    height = image.memoryHeight;
    width = (image.memoryWidth % 8 == 0) ? (image.memoryWidth / 8)
                                         : (image.memoryWidth / 8 + 1);
    uint32_t offset = image.imageOffset;
    uint32_t addr = 0;

    for (yPoint = 0; yPoint < height; yPoint++) {
        for (xPoint = 0; xPoint < width; xPoint++) {  // 8 pixel =  1 byte
            addr = xPoint + yPoint * width + offset;
            mySPI.writeByte(addr, pgm_read_byte(imageBuffer + xPoint + yPoint * width));
        }
    }
}
