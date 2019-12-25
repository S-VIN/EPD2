#include "RY4in2.h"

void RY4in2 ::turnOnDisplay(void) {
    sendCommand(0x12);  // DISPLAY_REFRESH
    delay(100);
    readBusy();
}

void RY4in2 ::init(void) {
    reset();

    sendCommand(0x06);  // BOOSTER_SOFT_START
    sendData(0x17);
    sendData(0x17);
    sendData(0x17);     // 07 0f 17 1f 27 2F 37 2f
    sendCommand(0x04);  // POWER_ON
    readBusy();
    sendCommand(0x00);  // PANEL_SETTING
    sendData(0x0F);     // LUT from OTP
}

void RY4in2 ::clear(void) {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    sendCommand(0x10);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
        }
    }

    sendCommand(0x13);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
        }
    }
    turnOnDisplay();
}

void RY4in2 ::display(void) {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    sendCommand(0x10);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(mySPI.readByte(i + j * width));
        }
    }

    sendCommand(0x13);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(mySPI.readByte(i + j * width + image.imageOffset));
        }
    }

    turnOnDisplay();
}

void RY4in2 ::sleep(void) {
    sendCommand(0x02);  // POWER_OFF
    readBusy();
    sendCommand(0x07);  // DEEP_SLEEP
    sendData(0XA5);
}
