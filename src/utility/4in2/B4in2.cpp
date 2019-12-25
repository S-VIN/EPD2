#include "B4in2.h"
#include "Display.h"

const unsigned char vcom[] = {
                              0x00, 0x17, 0x00, 0x00, 0x00, 0x02,
                              0x00, 0x17, 0x17, 0x00, 0x00, 0x02,
                              0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,
                              0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00
};
const unsigned char ww[] = {
                          0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
                          0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
                          0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
                          0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
const unsigned char bw[] = {
    0x40,
    0x17,
    0x00,
    0x00,
    0x00,
    0x02,
    0x90,
    0x17,
    0x17,
    0x00,
    0x00,
    0x02,
    0x40,
    0x0A,
    0x01,
    0x00,
    0x00,
    0x01,
    0xA0,
    0x0E,
    0x0E,
    0x00,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};
const unsigned char wb[] = {
    0x80,
    0x17,
    0x00,
    0x00,
    0x00,
    0x02,
    0x90,
    0x17,
    0x17,
    0x00,
    0x00,
    0x02,
    0x80,
    0x0A,
    0x01,
    0x00,
    0x00,
    0x01,
    0x50,
    0x0E,
    0x0E,
    0x00,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};
const unsigned char bb[] = {
    0x80,
    0x17,
    0x00,
    0x00,
    0x00,
    0x02,
    0x90,
    0x17,
    0x17,
    0x00,
    0x00,
    0x02,
    0x80,
    0x0A,
    0x01,
    0x00,
    0x00,
    0x01,
    0x50,
    0x0E,
    0x0E,
    0x00,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

void B4in2 ::turnOnDisplay(void) {
    sendCommand(0x12);
    delay(100);
    readBusy();
}

void B4in2 ::setLut(void) {
    uint16_t count;
    sendCommand(0x20);  //g vcom
    for (count = 0; count < 44; count++) {
        sendData(vcom[count]);
    }

    sendCommand(0x21);
    for (count = 0; count < 42; count++) {
        sendData(ww[count]);
    }

    sendCommand(0x22);
    for (count = 0; count < 42; count++) {
        sendData(bw[count]);
    }

    sendCommand(0x23);
    for (count = 0; count < 42; count++) {
        sendData(wb[count]);
    }

    sendCommand(0x24);
    for (count = 0; count < 42; count++) {
        sendData(bb[count]);
    }
}

void B4in2 ::init(void) {
    reset();

    sendCommand(0x01);  // POWER SETTING
    sendData(0x03);
    sendData(0x00);
    sendData(0x2b);
    sendData(0x2b);

    sendCommand(0x06);  // boost soft start
    sendData(0x17);     //A
    sendData(0x17);     //B
    sendData(0x17);     //C

    sendCommand(0x04);  // POWER_ON
    readBusy();

    sendCommand(0x00);  // panel setting
    sendData(0xbf);     // KW-BF   KWR-AF	BWROTP 0f	BWOTP 1f
    sendData(0x0d);

    sendCommand(0x30);  // PLL setting
    sendData(0x3C);     // 3A 100HZ   29 150Hz 39 200HZ	31 171HZ

    sendCommand(0x61);  // resolution setting
    sendData(0x01);
    sendData(0x90);  //128
    sendData(0x01);  //
    sendData(0x2c);

    sendCommand(0x82);  // vcom_DC setting
    sendData(0x28);

    sendCommand(0X50);  // VCOM AND DATA INTERVAL SETTING
    sendData(0x97);     // 97white border 77black border		VBDF 17|D7 VBDW 97 VBDB 57		VBDF F7 VBDW 77 VBDB 37  VBDR B7

    setLut();
}

void B4in2 ::clear(void) {
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

void B4in2 ::display(void) {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    sendCommand(0x13);
    for (uint16_t j = 0; j < height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(mySPI.readByte(i + j * width));
        }
    }

    turnOnDisplay();
}

void B4in2 ::sleep(void) {
    sendCommand(0x02);  // POWER_OFF
    readBusy();
    sendCommand(0x07);  // DEEP_SLEEP
    sendData(0XA5);
}
