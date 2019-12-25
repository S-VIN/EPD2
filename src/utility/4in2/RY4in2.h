#pragma once

#include <Display.h>

class RY4in2 : public Display {
   public:
    RY4in2() : Display(displayWidthRY4in2, displayHeightRY4in2) {}
    static const uint16_t displayWidthRY4in2 = 400;
    static const uint16_t displayHeightRY4in2 = 300;
    void init(void);
    void clear(void);
    void display(void);
    void sleep(void);
    void turnOnDisplay(void);
};
