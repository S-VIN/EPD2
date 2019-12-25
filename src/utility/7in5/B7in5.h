#pragma once

#include "Display.h"

class B7in5 : public Display {
   public:
    B7in5() : Display(displayWidthB7in5, displayHeightB7in5) {}
    static const int displayWidthB7in5 = 640;
    static const int displayHeightB7in5 = 384;
    void init(void);
    void clear(void);
    void display(void);
    void sleep(void);
    void turnOnDisplay(void);
    void setColor(uint8_t color);
    void clearWindow(uint16_t color);
};
