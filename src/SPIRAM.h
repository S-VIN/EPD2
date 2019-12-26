#pragma once
#include <SPI.h>

const uint8_t CMD_WREN 0x06;
const uint8_t CMD_WRDI 0x04;
const uint8_t CMD_RDSR 0x05;
const uint8_t CMD_WRSR 0x01;
const uint8_t CMD_READ 0x03;
const uint8_t CMD_WRITE 0x02;

const uint8_t BYTE_MODE 0x00;
const uint8_t PAGE_MODE 0x80;
const uint8_t STREAM_MODE 0x40;

    class SPIRAM {
   private:

   public:
    SPIRAM(uint8_t csPin = 5);

    void setMode(uint8_t mode);

    uint8_t readByte(uint32_t addr);
    void writeByte(uint32_t addr, uint8_t data);

    void readPage(uint32_t addr, uint8_t *pBuf);
    void writePage(uint32_t addr, uint8_t *pBuf);

    void readStream(uint32_t addr, uint8_t *pBuf, uint32_t len);
    void writeStream(uint32_t addr, uint8_t *pBuf, uint32_t len);

    void userTest(void);

    ~SPIRAM();
};
