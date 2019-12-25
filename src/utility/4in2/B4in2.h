#pragma once

#include "Display.h"

class B4in2 : public Display {
   public:
    B4in2() : Display(displayWidthB4in2, displayHeightB4in2) {}
    static const uint16_t displayWidthB4in2 = 400;
    static const uint16_t displayHeightB4in2 = 300;
    void init(void);
    void clear(void);
    void display(void);
    void sleep(void);
    void turnOnDisplay(void);
    void setLut(void);
};

