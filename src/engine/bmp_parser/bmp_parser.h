//
// Created by berserker on 8/3/26.
//

#ifndef GQUEST_BMP_PARSER_H
#define GQUEST_BMP_PARSER_H


#include "../main/game.h"

texture_t *read_bmp_file(char *path);

#pragma pack(push, 1)
def {
  u16 magic;
  u32 fileSize;
  u32 reserved;
  u32 pixelOffset;
}bmp_header_t;
#pragma pack(pop)

#pragma pack(push, 1)
def {
  u32 headerSize;

  i32 width;
  i32 height;

  u16 planes;
  u16 bitsPerPixel;

  u32 compression;
  u32 imageSize;

  i32 xPixelsPerMeter;
  i32 yPixelsPerMeter;

  u32 colorsUsed;
  u32 importantColors;
}bitmap_header_t;
#pragma pack(pop)

#endif //GQUEST_BMP_PARSER_H
