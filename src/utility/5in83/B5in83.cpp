#include "B5in83.h"

void B5in83::turnOnDisplay(void) {
    sendCommand(0x12);  // DISPLAY_REFRESH
    delay(100);
    readBusy();
}

void B5in83::init(void) {
    reset();

    sendCommand(0x01);  // POWER_SETTING
    sendData(0x37);
    sendData(0x00);

    sendCommand(0x00);  // PANEL_SETTING
    sendData(0xCF);
    sendData(0x08);

    sendCommand(0x06);  // BOOSTER_SOFT_START
    sendData(0xc7);
    sendData(0xcc);
    sendData(0x28);

    sendCommand(0x04);  // POWER_ON
    readBusy();

    sendCommand(0x30);  // PLL_CONTROL
    sendData(0x3c);

    sendCommand(0x41);  // TEMPERATURE_CALIBRATION
    sendData(0x00);

    sendCommand(0x50);  // VCOM_AND_DATA_INTERVAL_SETTING
    sendData(0x77);

    sendCommand(0x60);  // TCON_SETTING
    sendData(0x22);

    sendCommand(0x61);             // TCON_RESOLUTION
    sendData(_width >> 8);  // source 600
    sendData(_width & 0xff);
    sendData(_height >> 8);  // gate 448
    sendData(_height & 0xff);

    sendCommand(0x82);  // VCM_DC_SETTING
    sendData(0x1E);     // decide by LUT file

    sendCommand(0xe5);  // FLASH MODE
    sendData(0x03);
}

void B5in83::clear(void) {
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

void B5in83::display(void) {
    uint8_t Data_Black, data;
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    sendCommand(0x10);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            Data_Black = mySPI.readByte(i + j * width);
            for (uint8_t k = 0; k < 8; k++) {
                if (Data_Black & 0x80)
                    data = 0x00;
                else
                    data = 0x03;
                data <<= 4;
                Data_Black <<= 1;
                k++;
                if (Data_Black & 0x80)
                    data |= 0x00;
                else
                    data |= 0x03;
                Data_Black <<= 1;
                sendData(data);
            }
        }
    }
    turnOnDisplay();
}

void B5in83::sleep(void) {
    sendCommand(0x02);  // POWER_OFF
    readBusy();
    sendCommand(0x07);  // DEEP_SLEEP
    sendData(0XA5);
}

void B5in83::setColor(uint8_t color){
    if(color == black){
        _color = white;
    }else{
        _color = black;
    }
}

void B5in83::clearWindow(uint16_t color) {
    if (color == black) {
        color = white;
    } else {
        color = black;
    }
    uint16_t height = image.memoryHeight;
    uint16_t width = (image.memoryWidth % 8 == 0) ? (image.memoryWidth / 8)
                                                  : (image.memoryWidth / 8 + 1);
    uint32_t offset = image.imageOffset;

    if (image.imageColor == color_inverted) {
        color = ~color;
    }
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {  // 8 pixel =  1 byte
            mySPI.writeByte((x + y * width) + offset, color);
        }
    }
}