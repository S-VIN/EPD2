#include "R1in54.h"

// SpiCommands B;
const unsigned char vcom[] = {0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A, 0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00};
const unsigned char w[] = {0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04};
const unsigned char b[] = {0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04};
const unsigned char g1[] = {0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04};
const unsigned char g2[] = {0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A, 0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04};
const unsigned char vcom1[] = {0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char red0[] = {0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const unsigned char red1[] = {0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void R1in54 ::setLutBw(void) {
    uint16_t count;
    sendCommand(0x20);  // g vcom
    for (count = 0; count < 15; count++) {
        sendData(vcom[count]);
    }
    sendCommand(0x21);  // g ww --
    for (count = 0; count < 15; count++) {
        sendData(w[count]);
    }
    sendCommand(0x22);  // g bw r
    for (count = 0; count < 15; count++) {
        sendData(b[count]);
    }
    sendCommand(0x23);  // g wb w
    for (count = 0; count < 15; count++) {
        sendData(g1[count]);
    }
    sendCommand(0x24);  // g bb b
    for (count = 0; count < 15; count++) {
        sendData(g2[count]);
    }
}

void R1in54 ::setLutRed(void) {
    uint16_t count;
    sendCommand(0x25);
    for (count = 0; count < 15; count++) {
        sendData(vcom1[count]);
    }
    sendCommand(0x26);
    for (count = 0; count < 15; count++) {
        sendData(red0[count]);
    }
    sendCommand(0x27);
    for (count = 0; count < 15; count++) {
        sendData(red1[count]);
    }
}

void R1in54 ::init(void) {
    reset();

    sendCommand(0x01);  // POWER_SETTING
    sendData(0x07);
    sendData(0x00);
    sendData(0x08);
    sendData(0x00);
    sendCommand(0x06);  // BOOSTER_SOFT_START
    sendData(0x07);
    sendData(0x07);
    sendData(0x07);
    sendCommand(0x04);  // POWER_ON

    readBusy();

    sendCommand(0X00);  // PANEL_SETTING
    sendData(0xcf);
    sendCommand(0X50);  // VCOM_AND_DATA_INTERVAL_SETTING
    sendData(0x37);     // 0xF0
    sendCommand(0x30);  // PLL_CONTROL
    sendData(0x39);
    sendCommand(0x61);  // TCON_RESOLUTION set x and y
    sendData(0xC8);     // 200
    sendData(0x00);     // y High eight: 0
    sendData(0xC8);     // y Low eight: 200
    sendCommand(0x82);  // VCM_DC_SETTING_REGISTER
    sendData(0xe);

    setLutBw();
    setLutRed();
}

void R1in54 ::clear(void) {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    // send black data
    sendCommand(0x10);  // DATA_START_TRANSMISSION_1
    delay(2);
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
            sendData(0xFF);
        }
    }
    delay(2);

    // send red data
    sendCommand(0x13);  // DATA_START_TRANSMISSION_2
    delay(2);
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
        }
    }
    delay(2);

    sendCommand(0x12);  // DISPLAY_REFRESH
    readBusy();
}

void R1in54 ::display() {
    uint8_t Temp = 0x00;
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8)
                              : (_width / 8 + 1);
    height = _height;

    sendCommand(0x10);  // DATA_START_TRANSMISSION_1
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            Temp = 0x00;
            for (int bit = 0; bit < 4; bit++) {
                if ((mySPI.readByte(i + j * width) & (0x80 >> bit)) != 0) {
                    Temp |= 0xC0 >> (bit * 2);
                }
            }
            sendData(Temp);
            Temp = 0x00;
            for (int bit = 4; bit < 8; bit++) {
                if ((mySPI.readByte(i + j * width) & (0x80 >> bit)) != 0) {
                    Temp |= 0xC0 >> ((bit - 4) * 2);
                }
            }
            sendData(Temp);
        }
    }
    delay(2);

    sendCommand(0x13);  // DATA_START_TRANSMISSION_2
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(mySPI.readByte(i + j * width + image.imageOffset));
            //sendData(SpiCommands::readByte(i + j * width + image.imageOffset));
        }
    }
    delay(2);

    // display refresh
    sendCommand(0x12);  // DISPLAY_REFRESH
    readBusy();
}

void R1in54 ::sleep(void) {
    sendCommand(0x50);  // VCOM_AND_DATA_INTERVAL_SETTING
    sendData(0x17);
    sendCommand(0x82);  // VCM_DC_SETTING_REGISTER
    sendData(0x00);
    sendCommand(0x01);  // POWER_SETTING
    sendData(0x02);
    sendData(0x00);
    sendData(0x00);
    sendData(0x00);
    readBusy();
    sendCommand(0x02);  // POWER_OFF
}
