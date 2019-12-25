#ifndef FONTS_H
#define FONTS_H

#define MAX_HEIGHT_FONT 41
#define MAX_WIDTH_FONT 32

#include <stdint.h>

// ASCII
struct Sfont {
  const uint8_t *table;
  uint16_t width;
  uint16_t height;
};

extern Sfont sFont24;
extern Sfont sFont20;
extern Sfont sFont16;
extern Sfont sFont12;
extern Sfont sFont8;

#endif