#include "gio.h"
#include "logging.h"
#include "../rendering/render.h"
#include "../main/game.h"
#include "../rendering/filehelper.h"

/***** add_shader *****/
i64 add_shader(shader_table_t *shader_table, shader_t *shader) {
  if (!shader_table) {
    logging.warn("shader table invalid"); return -1;
  }
  if (!shader) {
    logging.warn("shader invalid"); return -1;
  }

  shader_table->nshaders++;
  shader_t *temp = realloc(shader_table->shader, sizeof(shader_t) * shader_table->nshaders);
  if (!temp) logging.error(NO_RETURN, "realloc failed to increase shader table!");
  shader_table->shader = temp;
  shader_table->shader[shader_table->nshaders - 1] = *shader;
  return (i64)shader_table->nshaders - 1;
}

/***** generate_shaders *****/
void generate_shaders(shader_table_t *shader_table) {
  shader_t temp_shader;
  temp_shader.vert_glsl = read_file("./src/engine/basic.vert");
  temp_shader.frag_glsl = read_file("./src/engine/basic.frag");
  add_shader(shader_table, &temp_shader);
}