#include "Y1in54.h"

void Y1in54 ::readBusy(void) {
    unsigned char busy;
    do {
        sendCommand(0x71);
        busy = digitalRead(EPD_BUSY);
        busy = !(busy & 0x01);
    } while (busy);
    delay(200);
}

void Y1in54 ::init(void) {
    reset();

    sendCommand(0x06);  // boost soft start
    sendData(0x17);
    sendData(0x17);
    sendData(0x17);
    sendCommand(0x04);

    readBusy();

    sendCommand(0x00);  // panel setting
    sendData(0x0f);     // LUT from OTP 160x296
    sendData(0x0d);     // VCOM to 0V fast

    sendCommand(0x61);  // resolution setting
    sendData(0x98);     // 152
    sendData(0x00);     // 152
    sendData(0x98);

    sendCommand(0X50);  // VCOM AND DATA INTERVAL SETTING
    sendData(0x77);     // WBmode:VBDF 17|D7 VBDW 97 VBDB 57
                        // WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

void Y1in54 ::clear(void) {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    // send black data
    sendCommand(0x10);
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
        }
    }

    // send red data
    sendCommand(0x13);
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
        }
    }

    // display refresh
    sendCommand(0x12);
    readBusy();
}

void Y1in54 ::display() {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    // send black data
    sendCommand(0x10);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(mySPI.readByte(i + j * width));
        }
    }

    // send red data
    sendCommand(0x13);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(mySPI.readByte(i + j * width + image.imageOffset));
        }
    }

    // display refresh
    sendCommand(0x12);
    readBusy();
}

void Y1in54 ::sleep(void) {
    sendCommand(0X02);  // power off
    readBusy();
    sendCommand(0X07);  // deep sleep
    sendData(0xA5);
}
