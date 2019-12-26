#pragma once
#include <SPI.h>

#ifndef __EPD_SDCARD_H__
#define __EPD_SDCARD_H__

#include <SD.h>
#include "DEV_Config.h"
#include "GUI_Paint.h"

typedef struct {
    UDOUBLE Size;
    UDOUBLE Index;
    UDOUBLE Header_Size;
    UDOUBLE Bit_Pixel;
    UDOUBLE BMP_Width;
    UDOUBLE BMP_Height;
} BMP_HEADER;
extern BMP_HEADER BMP_Header;

UWORD SDCard_Read16(File f);
UDOUBLE SDCard_Read32(File f);
boolean SDCard_ReadBmpHeader(File f);

void SDCard_Init(void);
void SDCard_ReadBMP(const char *BmpName, UWORD Xstart, UWORD Ystart);

#endif