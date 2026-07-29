#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <stdbool.h>

#include "physics.h"
#include "game.h"
#include "matrix.h"


void start_render(void);

def {
  float x, y, z;
}vert_t;

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
    GLuint program;

    GLint projection;
    GLint model;
    GLint view;

}shader_t;

def {
    mesh_t mesh;
    shader_t shader;

    vec3 pos;

    bool remodel;
    mat4 model;
}render_object_t;

void start_render(void);
void gl_error_check(void);
void update_model(render_object_t *object);
void draw_object(render_object_t *object, camera_t *camera);







#define demo_tri_\
\
  free(vertices);\
  vertices = malloc(sizeof(vertex_t)*3);\
  vertices[0] = (vertex_t){{ 1 , 1 , 0 }, {0, 0}};\
  vertices[1] = (vertex_t){{ 0 ,-1 , 0 }, {1, 0}};\
  vertices[2] = (vertex_t){{-1 , 1 , 0 }, {0, 1}};\
\
  free(triangles);\
  triangles = malloc(sizeof(tri_t)*1);\
  triangles[0] = (tri_t){ 0 , 1 , 2 };\
\
  render_object_t demo_tri;\
  demo_tri.shader = init_shader("./src/render/basic.vert",\
                                "./src/render/basic.frag");\
  demo_tri.mesh   = init_mesh(3, vertices, 1, triangles);\
\
  demo_tri.pos    = (vec3){0, 3,-10};\
                               \
  demo_tri.remodel  = 1;\






  
  #define ground_                                       \
  \
  free(vertices);\
  vertices = malloc(sizeof(vertex_t)*4);\
  vertices[0] = (vertex_t){{-32, 0 ,-32}, {0.33f, 0.7f}};\
  vertices[1] = (vertex_t){{ 32, 0 ,-32}, {0.33f, 0.7f}};\
  vertices[2] = (vertex_t){{ 32, 0 , 32}, {0.33f, 0.7f}};\
  vertices[3] = (vertex_t){{-32, 0 , 32}, {0.33f, 0.7f}};\
\
  free(triangles);\
  triangles = malloc(sizeof(tri_t)*2);\
  triangles[0] = (tri_t){ 0 , 1 , 2 };\
  triangles[1] = (tri_t){ 0 , 2 , 3 };\
\
  render_object_t ground;\
  ground.shader = init_shader("./src/render/basic.vert",\
                                "./src/render/basic.frag");\
  ground.mesh   = init_mesh(4, vertices, 2, triangles);\
\
  ground.pos    = (vec3){ 0 , 0 , 0 };\
                               \
  ground.remodel  = 1;\

  



#define tower_                                                        \
                                                                     \
  free(vertices);                                                    \
  vertices = malloc(sizeof(vertex_t)*32);                            \
                                                                     \
  /* tower bottom ring */                                            \
  vertices[0] = (vertex_t){{  2, 0, -5 },{0.3f, 0.999f}};             \
  vertices[1] = (vertex_t){{ -2, 0, -5 },{0.3f, 0.999f}};             \
  vertices[2] = (vertex_t){{ -5, 0, -2 },{0.3f, 0.999f}};             \
  vertices[3] = (vertex_t){{ -5, 0,  2 },{0.3f, 0.999f}};             \
  vertices[4] = (vertex_t){{ -2, 0,  5 },{0.3f, 0.999f}};             \
  vertices[5] = (vertex_t){{  2, 0,  5 },{0.3f, 0.999f}};             \
  vertices[6] = (vertex_t){{  5, 0,  2 },{0.3f, 0.999f}};             \
  vertices[7] = (vertex_t){{  5, 0, -2 },{0.3f, 0.999f}};             \
                                                                     \
  /* tower top ring (also roof base later) */                         \
  vertices[8]  = (vertex_t){{  2,15,-5 },{0.3f, 0.999f}};             \
  vertices[9]  = (vertex_t){{ -2,15,-5 },{0.3f, 0.999f}};             \
  vertices[10] = (vertex_t){{ -5,15,-2 },{0.3f, 0.999f}};             \
  vertices[11] = (vertex_t){{ -5,15, 2 },{0.3f, 0.999f}};             \
  vertices[12] = (vertex_t){{ -2,15, 5 },{0.3f, 0.999f}};             \
  vertices[13] = (vertex_t){{  2,15, 5 },{0.3f, 0.999f}};             \
  vertices[14] = (vertex_t){{  5,15, 2 },{0.3f, 0.999f}};             \
  vertices[15] = (vertex_t){{  5,15,-2 },{0.3f, 0.999f}};             \
                                                                     \
  /* turret lower ring (overhang, same y as tower top) */             \
  vertices[16] = (vertex_t){{  2,15,-6 },{0.3f, 0.999f}};             \
  vertices[17] = (vertex_t){{ -2,15,-6 },{0.3f, 0.999f}};             \
  vertices[18] = (vertex_t){{ -6,15,-2 },{0.3f, 0.999f}};             \
  vertices[19] = (vertex_t){{ -6,15, 2 },{0.3f, 0.999f}};             \
  vertices[20] = (vertex_t){{ -2,15, 6 },{0.3f, 0.999f}};             \
  vertices[21] = (vertex_t){{  2,15, 6 },{0.3f, 0.999f}};             \
  vertices[22] = (vertex_t){{  6,15, 2 },{0.3f, 0.999f}};             \
  vertices[23] = (vertex_t){{  6,15,-2 },{0.3f, 0.999f}};             \
                                                                     \
  /* turret upper ring (same footprint, +1 y) */                      \
  vertices[24] = (vertex_t){{  2,16,-6 },{0.3f, 0.999f}};             \
  vertices[25] = (vertex_t){{ -2,16,-6 },{0.3f, 0.999f}};             \
  vertices[26] = (vertex_t){{ -6,16,-2 },{0.3f, 0.999f}};             \
  vertices[27] = (vertex_t){{ -6,16, 2 },{0.3f, 0.999f}};             \
  vertices[28] = (vertex_t){{ -2,16, 6 },{0.3f, 0.999f}};             \
  vertices[29] = (vertex_t){{  2,16, 6 },{0.3f, 0.999f}};             \
  vertices[30] = (vertex_t){{  6,16, 2 },{0.3f, 0.999f}};             \
  vertices[31] = (vertex_t){{  6,16,-2 },{0.3f, 0.999f}};             \
  free(triangles);                                                   \
  triangles = malloc(sizeof(tri_t)*48);                              \
                                                                     \
  /* tower walls: bottom ring -> tower top ring */                    \
  triangles[0]  = (tri_t){0,1,9};                                    \
  triangles[1]  = (tri_t){0,9,8};                                    \
                                                                     \
  triangles[2]  = (tri_t){1,2,10};                                   \
  triangles[3]  = (tri_t){1,10,9};                                   \
                                                                     \
  triangles[4]  = (tri_t){2,3,11};                                   \
  triangles[5]  = (tri_t){2,11,10};                                  \
                                                                     \
  triangles[6]  = (tri_t){3,4,12};                                   \
  triangles[7]  = (tri_t){3,12,11};                                  \
                                                                     \
  triangles[8]  = (tri_t){4,5,13};                                   \
  triangles[9]  = (tri_t){4,13,12};                                  \
                                                                     \
  triangles[10] = (tri_t){5,6,14};                                   \
  triangles[11] = (tri_t){5,14,13};                                  \
                                                                     \
  triangles[12] = (tri_t){6,7,15};                                   \
  triangles[13] = (tri_t){6,15,14};                                  \
                                                                     \
  triangles[14] = (tri_t){7,0,8};                                    \
  triangles[15] = (tri_t){7,8,15};                                   \
                                                                     \
  /* turret underside: tower top ring -> turret lower ring */         \
  triangles[16] = (tri_t){8,16,17};                                  \
  triangles[17] = (tri_t){8,17,9};                                   \
                                                                     \
  triangles[18] = (tri_t){9,17,18};                                  \
  triangles[19] = (tri_t){9,18,10};                                  \
                                                                     \
  triangles[20] = (tri_t){10,18,19};                                 \
  triangles[21] = (tri_t){10,19,11};                                 \
                                                                     \
  triangles[22] = (tri_t){11,19,20};                                 \
  triangles[23] = (tri_t){11,20,12};                                 \
                                                                     \
  triangles[24] = (tri_t){12,20,21};                                 \
  triangles[25] = (tri_t){12,21,13};                                 \
                                                                     \
  triangles[26] = (tri_t){13,21,22};                                 \
  triangles[27] = (tri_t){13,22,14};                                 \
                                                                     \
  triangles[28] = (tri_t){14,22,23};                                 \
  triangles[29] = (tri_t){14,23,15};                                 \
                                                                     \
  triangles[30] = (tri_t){15,23,16};                                 \
  triangles[31] = (tri_t){15,16,8};                                  \
  /* turret outer walls: lower ring -> upper ring */                   \
  triangles[32] = (tri_t){16,17,25};                                 \
  triangles[33] = (tri_t){16,25,24};                                 \
                                                                     \
  triangles[34] = (tri_t){17,18,26};                                 \
  triangles[35] = (tri_t){17,26,25};                                 \
                                                                     \
  triangles[36] = (tri_t){18,19,27};                                 \
  triangles[37] = (tri_t){18,27,26};                                 \
                                                                     \
  triangles[38] = (tri_t){19,20,28};                                 \
  triangles[39] = (tri_t){19,28,27};                                 \
                                                                     \
  triangles[40] = (tri_t){20,21,29};                                 \
  triangles[41] = (tri_t){20,29,28};                                 \
                                                                     \
  triangles[42] = (tri_t){21,22,30};                                 \
  triangles[43] = (tri_t){21,30,29};                                 \
                                                                     \
  triangles[44] = (tri_t){22,23,31};                                 \
  triangles[45] = (tri_t){22,31,30};                                 \
                                                                     \
  triangles[46] = (tri_t){23,16,24};                                 \
  triangles[47] = (tri_t){23,24,31};                                 \
                                                                     \
  render_object_t tower;                                             \
  tower.shader = init_shader("./src/render/basic.vert",              \
                              "./src/render/basic.frag");            \
                                                                     \
  tower.mesh = init_mesh(32, vertices, 48, triangles);                \
                                                                     \
  tower.pos = (vec3){-20,0,-20};                                     \
                                                                     \
  tower.remodel = 1;
  
#define roof_                                                        \
                                                                     \
  free(vertices);                                                    \
  vertices = malloc(sizeof(vertex_t)*9);                             \
                                                                     \
  /* roof base ring - identical to tower top ring */                 \
  vertices[0] = (vertex_t){{  2,15,-5 },{0.078f,0.647f}};              \
  vertices[1] = (vertex_t){{ -2,15,-5 },{0.078f,0.647f}};              \
  vertices[2] = (vertex_t){{ -5,15,-2 },{0.078f,0.647f}};              \
  vertices[3] = (vertex_t){{ -5,15, 2 },{0.078f,0.647f}};              \
  vertices[4] = (vertex_t){{ -2,15, 5 },{0.078f,0.647f}};              \
  vertices[5] = (vertex_t){{  2,15, 5 },{0.078f,0.647f}};              \
  vertices[6] = (vertex_t){{  5,15, 2 },{0.078f,0.647f}};              \
  vertices[7] = (vertex_t){{  5,15,-2 },{0.078f,0.647f}};              \
                                                                     \
  /* roof peak */                                                    \
  vertices[8] = (vertex_t){{0,25,0},{0.078f,0.647f}};                  \
                                                                     \
  free(triangles);                                                   \
  triangles = malloc(sizeof(tri_t)*8);                               \
                                                                     \
  triangles[0] = (tri_t){0,1,8};                                     \
  triangles[1] = (tri_t){1,2,8};                                     \
  triangles[2] = (tri_t){2,3,8};                                     \
  triangles[3] = (tri_t){3,4,8};                                     \
  triangles[4] = (tri_t){4,5,8};                                     \
  triangles[5] = (tri_t){5,6,8};                                     \
  triangles[6] = (tri_t){6,7,8};                                     \
  triangles[7] = (tri_t){7,0,8};                                     \
                                                                     \
  render_object_t roof;                                              \
  roof.shader = init_shader("./src/render/basic.vert",               \
                             "./src/render/basic.frag");             \
                                                                     \
  roof.mesh = init_mesh(9, vertices, 8, triangles);                   \
                                                                     \
  roof.pos = (vec3){-20,0,-20};                                      \
                                                                     \
  roof.remodel = 1;


#endif