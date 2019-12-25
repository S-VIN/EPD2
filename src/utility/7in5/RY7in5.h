#pragma once

#include "Display.h"

class RY7in5 : public Display {
   public:
    RY7in5() : Display(displayWidthRY1in5, displayHeightRY1in5) {}

    static const int displayWidthRY1in5 = 640;
    static const int displayHeightRY1in5 = 384;

    void init(void);
    void clear(void);
    void display(void);
    void sleep(void);
    void readBusy(void);
    void displayHalfScreen(void);
    void turnOnDisplay(void);
};

