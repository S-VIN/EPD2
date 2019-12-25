#pragma once

#include "Display.h"

class R1in54 : public Display {
   public:
    static const int displayWidthR1in54 = 200;
    static const int displayHeightR1in54 = 200;
    R1in54() : Display(displayWidthR1in54, displayHeightR1in54) {}
    void init(void);
    void clear(void);
    void display(void);
    void sleep(void);
    void setLutBw(void);
    void setLutRed(void);
};
