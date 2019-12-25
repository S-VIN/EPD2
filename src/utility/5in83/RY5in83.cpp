#include "RY5in83.h"

void RY5in83 ::turnOnDisplay(void) {
    sendCommand(0x04);  // POWER ON
    readBusy();
    sendCommand(0x12);  // display refresh
    delay(100);
    readBusy();
}

void RY5in83 ::init(void) {
    reset();

    sendCommand(0x01);  // POWER_SETTING
    sendData(0x37);
    sendData(0x00);

    sendCommand(0x00);  // PANEL_SETTING
    sendData(0xCF);
    sendData(0x08);

    sendCommand(0x30);     // PLL_CONTROL
    sendData(0x3A);  //PLL:  0-15:0x3C, 15+:0x3A

    sendCommand(0x82);     // VCM_DC_SETTING
    sendData(0x28);  //all temperature  range

    sendCommand(0x06);  // BOOSTER_SOFT_START
    sendData(0xc7);
    sendData(0xcc);
    sendData(0x15);

    sendCommand(0x50);  // VCOM AND DATA INTERVAL SETTING
    sendData(0x77);

    sendCommand(0x60);  // TCON_SETTING
    sendData(0x22);

    sendCommand(0x65);  // FLASH CONTROL
    sendData(0x00);

    sendCommand(0x61);     // TCON_RESOLUTION
    sendData(0x02);  // source 600
    sendData(0x58);
    sendData(0x01);  // gate 448
    sendData(0xc0);

    sendCommand(0xe5);  // FLASH MODE
    sendData(0x03);
}

void RY5in83 ::clear(void) {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    sendCommand(0x10);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            for (uint8_t k = 0; k < 4; k++) {
                sendData(0x33);
            }
        }
    }

    turnOnDisplay();
}

void RY5in83 ::display(void) {
    uint8_t Data_Black, Data_RY, data;
    uint32_t i, j, width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;
    sendCommand(0x10);
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            Data_Black = mySPI.readByte(i + j * width);
            Data_RY = mySPI.readByte(i + j * width + image.imageOffset);  // red or yellow
            for (uint8_t k = 0; k < 8; k++) {
                if ((Data_RY & 0x80) == 0x00) {
                    data = 0x04;  //red
                } else if ((Data_Black & 0x80) == 0x00) {
                    data = 0x00;  //black
                } else {
                    data = 0x03;  //white
                }
                data = (data << 4) & 0xFF;
                Data_Black = (Data_Black << 1) & 0xFF;
                Data_RY = (Data_RY << 1) & 0xFF;
                k += 1;

                if ((Data_RY & 0x80) == 0x00) {
                    data |= 0x04;  //red
                } else if ((Data_Black & 0x80) == 0x00) {
                    data |= 0x00;  //black
                } else {
                    data |= 0x03;  //white
                }
                Data_Black = (Data_Black << 1) & 0xFF;
                Data_RY = (Data_RY << 1) & 0xFF;
                sendData(data);
            }
        }
    }
    turnOnDisplay();
}

void RY5in83 ::displayHalfScreen(void) {
    uint8_t Data_Black, Data_RY, data;
    uint32_t i, j, width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    sendCommand(0x10);
    for (j = 0; j < height / 2; j++) {
        for (i = 0; i < width; i++) {
            Data_Black = mySPI.readByte(i + j * width);
            Data_RY = mySPI.readByte(i + j * width + image.imageOffset);  // red or yellow
            for (uint8_t k = 0; k < 8; k++) {
                if ((Data_RY & 0x80) == 0x00) {
                    data = 0x04;  //red
                } else if ((Data_Black & 0x80) == 0x00) {
                    data = 0x00;  //black
                } else {
                    data = 0x03;  //white
                }
                data = (data << 4) & 0xFF;
                Data_Black = (Data_Black << 1) & 0xFF;
                Data_RY = (Data_RY << 1) & 0xFF;
                k += 1;

                if ((Data_RY & 0x80) == 0x00) {
                    data |= 0x04;  //red
                } else if ((Data_Black & 0x80) == 0x00) {
                    data |= 0x00;  //black
                } else {
                    data |= 0x03;  //white
                }
                Data_Black = (Data_Black << 1) & 0xFF;
                Data_RY = (Data_RY << 1) & 0xFF;
                sendData(data);
            }
        }
    }

    for (j = 0; j < height / 2; j++) {
        for (i = 0; i < width; i++) {
            for (uint8_t k = 0; k < 4; k++) {
                sendData(0x33);
            }
        }
    }

    turnOnDisplay();
}

void RY5in83 ::sleep(void) {
    sendCommand(0x02);  // POWER_OFF
    readBusy();
    sendCommand(0x07);  // DEEP_SLEEP
    sendData(0XA5);
}
