#include "B1in54.h"

const unsigned char fullUpdate[] = {
    0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22,
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88,
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51,
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00};

const unsigned char particalUpdate[] = {
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void B1in54::setWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd) {
    sendCommand(0x44);  // SET_RAM_X_ADDRESS_START_END_POSITION
    sendData((xStart >> 3) & 0xFF);
    sendData((xEnd >> 3) & 0xFF);

    sendCommand(0x45);  // SET_RAM_Y_ADDRESS_START_END_POSITION
    sendData(yStart & 0xFF);
    sendData((yStart >> 8) & 0xFF);
    sendData(yEnd & 0xFF);
    sendData((yEnd >> 8) & 0xFF);
}

void B1in54::setCursor(uint16_t xStart, uint16_t yStart) {
    sendCommand(0x4E);  // SET_RAM_X_ADDRESS_COUNTER
    sendData((xStart >> 3) & 0xFF);

    sendCommand(0x4F);  // SET_RAM_Y_ADDRESS_COUNTER
    sendData(yStart & 0xFF);
    sendData((yStart >> 8) & 0xFF);
    readBusy();
}

void B1in54::turnOnDisplay(void) {
    sendCommand(0x22);  // DISPLAY_UPDATE_CONTROL_2
    sendData(0xC4);
    sendCommand(0x20);  // MASTER_ACTIVATION
    sendCommand(0xFF);  // TERMINATE_FRAME_READ_WRITE
    readBusy();
}

void B1in54::init(uint8_t mode) {
    reset();

    sendCommand(0x01);  // DRIVER_OUTPUT_CONTROL
    sendData((_height - 1) & 0xFF);
    sendData(((_height - 1) >> 8) & 0xFF);
    sendData(0x00);  // GD = 0; SM = 0; TB = 0;

    sendCommand(0x0C);  // BOOSTER_SOFT_START_CONTROL
    sendData(0xD7);
    sendData(0xD6);
    sendData(0x9D);

    sendCommand(0x2C);  // WRITE_VCOM_REGISTER
    sendData(0xA8);     // VCOM 7C

    sendCommand(0x3A);  // SET_DUMMY_LINE_PERIOD
    sendData(0x1A);     // 4 dummy lines per gate

    sendCommand(0x3B);  // SET_GATE_TIME
    sendData(0x08);     // 2us per line

    sendCommand(0x11);
    sendData(0x03);

    //set the look-up table register
    sendCommand(0x32);
    if (mode == fullMode) {
        for (uint16_t i = 0; i < 30; i++) {
            sendData(fullUpdate[i]);
        }
    } else if (mode == partMode) {
        for (uint16_t i = 0; i < 30; i++) {
            sendData(particalUpdate[i]);
        }
    }
}

void B1in54::clear(void) {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;
    setWindow(0, 0, _width, _height);
    for (uint16_t j = 0; j < height; j++) {
        setCursor(0, j);
        sendCommand(0x24);
        for (uint16_t i = 0; i < width; i++) {
            sendData(0XFF);
        }
    }
    turnOnDisplay();
}

void B1in54::display() {
    uint16_t width, height;
    width = (_width % 8 == 0) ? (_width / 8) : (_width / 8 + 1);
    height = _height;

    uint32_t Addr = 0;
    // uint32_t Offset = ImageName;
    setWindow(0, 0, _width, _height);
    for (uint16_t j = 0; j < height; j++) {
        setCursor(0, j);
        sendCommand(0x24);
        for (uint16_t i = 0; i < width; i++) {
            Addr = i + j * width;
            sendData(mySPI.readByte(Addr));
        }
    }
    turnOnDisplay();
}

void B1in54::sleep(void) {
    sendCommand(0x10);
    sendData(0x01);
    readBusy();
    delay(1000);
    digitalWrite(EPD_RST, 0);
}
