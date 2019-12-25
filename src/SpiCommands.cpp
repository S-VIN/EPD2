#include "SpiCommands.h"

void SpiCommands ::init(void) {
    pinMode(SPIRAM_CS, OUTPUT);
    pinMode(EPD_CS, OUTPUT);
    pinMode(EPD_DC, OUTPUT);
    pinMode(EPD_RST, OUTPUT);
    pinMode(EPD_BUSY, INPUT);

    digitalWrite(EPD_CS, HIGH);
    digitalWrite(SPIRAM_CS, HIGH);
    // SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
    SPI.begin();
}

void SpiCommands ::exit(void) {
    digitalWrite(EPD_CS, HIGH);
    digitalWrite(SPIRAM_CS, HIGH);
    digitalWrite(EPD_RST, LOW);
}

// transfer mode
void SpiCommands ::setMode(uint8_t mode) {
    digitalWrite(SPIRAM_CS, LOW);

    SPI.transfer(CMD_WRSR);
    SPI.transfer(mode);

    digitalWrite(SPIRAM_CS, HIGH);
}

// transfer byte from EInc
uint8_t SpiCommands ::readByte(uint32_t addr) {
    uint8_t RD_Byte;

    digitalWrite(SPIRAM_CS, LOW);
    SPI.transfer(CMD_READ);

    SPI.transfer((uint8_t)(addr >> 16));
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)addr);

    RD_Byte = SPI.transfer(0x00);
    digitalWrite(SPIRAM_CS, HIGH);

    return RD_Byte;
}

// transfer byte on EInc
void SpiCommands ::writeByte(uint32_t addr, const uint8_t data) {
    // Write addr,data
    digitalWrite(SPIRAM_CS, LOW);

    SPI.transfer(CMD_WRITE);

    SPI.transfer((uint8_t)(addr >> 16));
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)addr);

    SPI.transfer(data);
    digitalWrite(SPIRAM_CS, HIGH);
}

// transfer pBuf from EInc
void SpiCommands ::readBuffer(uint32_t addr, uint8_t *pBuf, uint32_t len) {
    uint16_t i;

    // Write addr, read data
    digitalWrite(SPIRAM_CS, LOW);
    SPI.transfer(CMD_READ);

    SPI.transfer((uint8_t)(addr >> 16));
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)addr);

    for (i = 0; i < len; i++) {
        *pBuf = SPI.transfer(0x00);
        pBuf++;
    }
    digitalWrite(SPIRAM_CS, HIGH);
}

// transfer pBuf on EInc
void SpiCommands ::writeBuffer(uint32_t addr, uint8_t *pBuf, uint32_t len) {
    uint16_t i;

    digitalWrite(SPIRAM_CS, LOW);
    SPI.transfer(CMD_WRITE);

    SPI.transfer((uint8_t)(addr >> 16));
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)addr);

    for (i = 0; i < len; i++) {
        SPI.transfer(*pBuf);
        pBuf++;
    }
    digitalWrite(SPIRAM_CS, HIGH);
}
