#pragma once
#include "SPIRAM.h"

void SPIRAM ::setMode(uint8_t mode) {
    digitalWrite(SPIRAM_CS, LOW);
    SPI.transfer(CMD_WRSR);
    SPI.transfer(mode);
    digitalWrite(SPIRAM_CS, HIGH);
}

uint8_t SPIRAM ::readByte(uint32_t addr) {
    uint8_t inputByte;
    digitalWrite(SPIRAM_CS, LOW);
    SPI.transfer(CMD_READ);
    SPI.transfer((uint8_t)(addr >> 16));
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)addr);
    inputByte = SPI.transfer(0x00);
    digitalWrite(SPIRAM_CS, HIGH);
    return inputByte;
}

void SPIRAM ::writeByte(uint32_t addr, const uint8_t data) {
    digitalWrite(SPIRAM_CS, LOW);
    SPI.transfer(CMD_WRITE);
    SPI.transfer((uint8_t)(addr >> 16));
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)addr);
    SPI.transfer(data);
    digitalWrite(SPIRAM_CS, HIGH);
}

void SPIRAM ::readPage(uint32_t addr, uint8_t *pBuf) {
    uint16_t i;
    digitalWrite(SPIRAM_CS, LOW);
    SPI.transfer(CMD_READ);
    SPI.transfer((uint8_t)(addr >> 16));
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)addr);
    for (i = 0; i < 32; i++) {
        *pBuf = SPI.transfer(0x00);
        pBuf++;
    }
    digitalWrite(SPIRAM_CS, HIGH);
}

void SPIRAM ::writePage(uint32_t addr, uint8_t *pBuf) {
    uint16_t i;
    digitalWrite(SPIRAM_CS, LOW);
    SPI.transfer(CMD_WRITE);
    SPI.transfer((uint8_t)(addr >> 16));
    SPI.transfer((uint8_t)(addr >> 8));
    SPI.transfer((uint8_t)addr);
    for (i = 0; i < 32; i++) {
        SPI.transfer(*pBuf);
        pBuf++;
    }
    digitalWrite(SPIRAM_CS, HIGH);
}

void SPIRAM ::readStream(uint32_t addr, uint8_t *pBuf, uint32_t len) {
    uint16_t i;
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

void SPIRAM ::writeStream(uint32_t addr, uint8_t *pBuf, uint32_t len) {
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
