#pragma once

#include "Display.h"

class B1in54 : public Display {
   private:
    const uint8_t fullMode = 0;
    const uint8_t partMode = 1;

   public:
    B1in54() : Display(displayWidthB1in54, displayHeightB1in54) {}
    static const int displayWidthB1in54 = 200;
    static const int displayHeightB1in54 = 200;

    void init(uint8_t mode = 0);
    void clear(void);
    void display(void);
    void sleep(void);
    void setWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
    void setCursor(uint16_t xStart, uint16_t yStart);
    void turnOnDisplay(void);
};
