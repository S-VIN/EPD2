#ifndef SPI_COMMANDS_H
#define SPI_COMMANDS_H

#include <SPI.h>
#include <Wire.h>
#include <stdint.h>

#define SPIRAM_CS 5
#define EPD_CS 10
#define EPD_DC 9
#define EPD_RST 8
#define EPD_BUSY 7

// SRAM opcodes
#define CMD_WREN 0x06
#define CMD_WRDI 0x04
#define CMD_RDSR 0x05
#define CMD_WRSR 0x01
#define CMD_READ 0x03
#define CMD_WRITE 0x02

// SRAM modes
#define BYTE_MODE 0x00
#define PAGE_MODE 0x80
#define STREAM_MODE 0x40

class SpiCommands {
   public:
    SpiCommands() {
        init();
    }

    ~SpiCommands() { exit(); }
    void setMode(uint8_t mode);

    uint8_t readByte(uint32_t addr);
    void writeByte(uint32_t addr, uint8_t data);

    void readBuffer(uint32_t addr, uint8_t *pBuf, uint32_t len);
    void writeBuffer(uint32_t addr, uint8_t *pBuf, uint32_t len);

    void init();
    void exit();
};

#endif
