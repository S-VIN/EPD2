#pragma once

#include "Display.h"

class RY5in83 : public Display {
   public:
    RY5in83() : Display(displayWidthRY5in83, displayHeightRY5in83) {}
    static const int displayWidthRY5in83 = 600;
    static const int displayHeightRY5in83 = 448;
    void init(void);
    void clear(void);
    void display(void);
    void sleep(void);
    void displayHalfScreen(void);
    void turnOnDisplay(void);
};
