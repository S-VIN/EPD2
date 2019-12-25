#pragma once

#include "Display.h"

class B5in83 : public Display {
   public:
    B5in83() : Display(displayWidthB5in83, displayHeightB5in83) {}
    static const uint16_t displayWidthB5in83 = 600;
    static const uint16_t displayHeightB5in83 = 448;
    void init(void);
    void clear(void);
    void display(void);
    void sleep(void);
    void turnOnDisplay(void);
    void setColor(uint8_t color);
    void clearWindow(uint16_t color);
};


