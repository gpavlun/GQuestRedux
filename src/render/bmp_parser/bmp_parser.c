#include "bmp_parser.h"

#include "logging.h"

texture_t *read_bmp_file(char *path) {
  if (!path) {
    logging.warn("bmp image path is not set!");
    return NULL;
  }
  FILE *bmp_file = fopen(path, "rb");
  if (!bmp_file){
    logging.warn("could not read bmp file!");
    return NULL;
  }

  bmp_header_t header;
  if (!fread(&header, sizeof(bmp_header_t), 1, bmp_file)) {
    logging.warn("bmp file is invalid!");
    return NULL;
  }

  u32 dib_header_size;
  fread(&dib_header_size, sizeof(u8), 4, bmp_file);
  if (dib_header_size<40) {
    logging.warn("unsupported bmp (40 minimum)!");
    return NULL;
  }
  fseek( bmp_file, -4, SEEK_CUR);

  bitmap_header_t bitmap_header;
  fread(&bitmap_header, sizeof(bitmap_header_t), 1, bmp_file);

  if (bitmap_header.planes != 1){
    logging.warn("unsupported bmp planes (cannot be 1)!");
    return NULL;
  }
  if (bitmap_header.compression != 0){
    logging.warn("unsupported bmp compression (must be 0)!");
    return NULL;
  }
  if (bitmap_header.bitsPerPixel != 24){
    logging.warn("unsupported bmp pixel resolution (24 required)!");
    return NULL;
  }

  fseek(bmp_file, header.pixelOffset, SEEK_SET);

  u32 bytesperpix = bitmap_header.bitsPerPixel / 8;
  u32 rowSize = bitmap_header.width * bytesperpix;
  u32 padding = (4 - (rowSize % 4)) % 4;

  u8 *image = malloc(
    bitmap_header.width *
    bitmap_header.height *
    bytesperpix
  );

  u8 *row;
  if (bitmap_header.height>0) {
    for (int i=bitmap_header.height-1; i >= 0; i--){
      fread(image + i*rowSize, rowSize, 1, bmp_file);
      fseek(bmp_file, padding, SEEK_CUR);

      row = image + i * rowSize;
      for (int x=0; x<bitmap_header.width; x++) {
        u8 *pixel = row + x * bytesperpix;

        u8 temp = pixel[0];
        pixel[0] = pixel[2];
        pixel[2] = temp;
      }
    }
  }else {
    for (int i = 0; i < abs(bitmap_header.height); i++){
      fread(image + i*rowSize, rowSize, 1, bmp_file);
      fseek(bmp_file, padding, SEEK_CUR);

      row = image + i * rowSize;
      for (int x=0; x<bitmap_header.width; x++) {
        u8 *pixel = row + x * bytesperpix;

        u8 temp = pixel[0];
        pixel[0] = pixel[2];
        pixel[2] = temp;
      }
    }
  }

  texture_t *texture = malloc(sizeof(texture_t));
  texture->pixels = image;
  texture->height = bitmap_header.height;
  texture->width = bitmap_header.width;

  fclose(bmp_file);

  return texture;
}
