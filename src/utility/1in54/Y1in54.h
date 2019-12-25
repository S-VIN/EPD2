#pragma once

#include "Display.h"

class Y1in54 : public Display {
   public:
    Y1in54() : Display(displayWidthY1in54, displayHeightY1in54){};
    const int displayWidthY1in54 = 152;
    const int displayHeightY1in54 = 152;
    void init(void);
    void clear(void);
    void display(void);
    void sleep(void);
    void readBusy(void);
};
