#include "logging.h"
#include "../main/game.h"
#include "../bmp_parser/bmp_parser.h"

i64 add_texture(texture_table_t *texture_table, texture_t *texture) {
  if (!texture_table) {
    logging.warn("texture table invalid"); return -1;
  }
  if (!texture) {
    logging.warn("texture invalid"); return -1;
  }

  texture_table->ntextures++;
  texture_t *temp = realloc(texture_table->texture, sizeof(texture_t) * texture_table->ntextures);
  if (!temp) logging.error(NO_RETURN, "realloc failed to increase texture table!");
  texture_table->texture = temp;
  texture_table->texture[texture_table->ntextures - 1] = *texture;

  return (i64)texture_table->ntextures - 1;
}



void generate_textures(texture_table_t *texture_table) {

  texture_t *texture;

  texture = read_bmp_file("./assets/bmp_textures/grass.bmp");
  add_texture(texture_table, texture);
  free(texture);
  texture = read_bmp_file("./assets/bmp_textures/bricks.bmp");
  add_texture(texture_table, texture);
  free(texture);
  texture = read_bmp_file("./assets/bmp_textures/shingles1.bmp");
  add_texture(texture_table, texture);
  free(texture);
  texture = read_bmp_file("./assets/bmp_textures/volcano.bmp");
  add_texture(texture_table, texture);
  free(texture);
  texture = read_bmp_file("./assets/bmp_textures/lava.bmp");
  add_texture(texture_table, texture);
  free(texture);
  texture = read_bmp_file("./assets/bmp_textures/needles.bmp");
  add_texture(texture_table, texture);
  free(texture);
}

