#ifndef RENDER_H
#define RENDER_H


#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include <glad/glad.h>
#include <logging.h>
#include <stdbool.h>

#include "../sdl_windowing/sdl_ops.h"
#include "../physics/physics.h"
#include "../main/game.h"
#include "../math/matrix.h"
#include "../math/vector.h"
#include "../main/typing.h"
#include "../tui_panel/terminal_interface.h"

#include "filehelper.h"
#include "lighting.h"

#define $chunk_quads 1024
#define $chunk_verts ($chunk_quads + 1)



def {
    u32 A, B;
}edge_t;

def {
    u16 a, b, c;
    hexcode_u color;
}triangle_t;



def {
    u8 wire_frame: 1;
    u8 triangles: 1;
    u8 vertices: 1;
}mesh_opts;


typedef struct {
    u8 r, g, b, a;
} rgba_t;


def {
  vec3 pos;
  vec3 normal;
  vec2 uv;
}vertex_t;

def {
  u32 a, b, c;
}tri_t;

def {
  size_t nverts;
  vertex_t *vert;
  size_t ntris;
  tri_t *tri;

  GLuint vao;
  GLuint vbo;
  GLuint ebo;

  mesh_opts opts;
}mesh_t;

def {
  char *vert_glsl;
  char *frag_glsl;

  GLuint program;

  GLint projection;
  GLint model;
  GLint view;

}shader_t;

def {
  bool hidden;

  mesh_t *mesh;
  shader_t *shader;

  bool remodel;
  mat4 model;

  vec3 pos;
  vec3 rot;
  vec3 scale;
}render_object_t;

def {
  render_object_t **object;
  size_t nobjects;
}render_array_t;


def {
  float *heightmap;
  size_t mesh_idx;
  vec3 pos;
} logical_chunk_t;
extern logical_chunk_t *glob_terrain;





void *start_render(void *arg);
void gl_error_check(void);
void update_model(render_object_t *object);
void draw_object(render_object_t *object, camera_t *camera, lighting_t *lighting);
void update_camera(camera_t *camera);

void gpu_upload_mesh(mesh_t *mesh);
void gpu_upload_shader(shader_t *shader);


shader_t init_shader(char *vert_path, char *frag_path);


#define demo_tri_ \
\
vertices = calloc(3, sizeof(vertex_t));\
\
vertices[0] = (vertex_t){\
  .pos = { 1,  1, 0 },\
  .normal = {0,0,1},\
  .uv = {0,0}\
};\
\
vertices[1] = (vertex_t){\
  .pos = { 0, -1, 0 },\
  .normal = {0,0,1},\
  .uv = {1,0}\
};\
\
vertices[2] = (vertex_t){\
  .pos = {-1,  1, 0 },\
  .normal = {0,0,1},\
  .uv = {0,1}\
};\
\
triangles = calloc(1, sizeof(tri_t));\
triangles[0] = (tri_t){2,1,0};



#define GRASS {0.33f, 0.7f}
#define STONE {0.3f, 0.999f}
#define WOOD {0.078f,0.647f}


#endif