#ifndef RENDER_H
#define RENDER_H

#include "../main/typing.h"

#include <glad/glad.h>
#include "../math/matrix.h"
#include "../math/vector.h"

def mesh_table_t mesh_table_t;
def shader_table_t shader_table_t;
def texture_table_t texture_table_t;

def entity_t entity_t;
def camera_t camera_t;
def lighting_t lighting_t;

#define $chunk_quads 1024
#define $chunk_verts ($chunk_quads + 1)




def rgba_t{
    u8 r, g, b, a;
} rgba_t;


def vertex_t{
  vec3 pos;
  vec3 normal;
  vec2 uv;
}vertex_t;

def tri_t{
  u32 a, b, c;
}tri_t;

def mesh_t{
  size_t nverts;
  vertex_t *vert;
  size_t ntris;
  tri_t *tri;

  GLuint vao;
  GLuint vbo;
  GLuint ebo;
}mesh_t;

def texture_t{
  GLuint handle;
  u8 *pixels;
  u32 width;
  u32 height;
}texture_t;

def shader_t{
  char *vert_glsl;
  char *frag_glsl;

  GLuint program;

  GLint projection;
  GLint model;
  GLint view;

}shader_t;

void *start_render(void *arg);
void gl_error_check(void);
void update_model(entity_t *entity);
void update_camera(camera_t *camera);



void draw_object(
  entity_t *entity,
  camera_t *camera,
  lighting_t *lighting,
  mesh_table_t *mesh_table,
  shader_table_t *shader_table,
  texture_table_t *texture_table
);


void gpu_upload_mesh(mesh_t *mesh);
void gpu_upload_shader(shader_t *shader);
void gpu_upload_texture(texture_t *texture);

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