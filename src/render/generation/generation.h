#ifndef GQUEST_GENERATION_H
#define GQUEST_GENERATION_H

#include "../main/typing.h"
#include <stddef.h>

def mesh_table_t mesh_table_t;
def shader_table_t shader_table_t;
def texture_table_t texture_table_t;

def mesh_t mesh_t;
def vertex_t vertex_t;
def tri_t tri_t;
def shader_t shader_t;
def texture_t texture_t;

def simulation_t simulation_t;

void generate_shaders(shader_table_t *shader_table);
i64 add_shader(shader_table_t *shader_table, shader_t *shader);

void generate_meshes(mesh_table_t *mesh_table);
void mesh_generate_normals(mesh_t *mesh);
mesh_t new_mesh(size_t nverts, vertex_t *verts, size_t ntris, tri_t *tris);
i64 add_mesh(mesh_table_t *mesh_table, mesh_t *mesh);

void generate_textures(texture_table_t *texture_table);
i64 add_texture(texture_table_t *texture_table, texture_t *texture);

float height_func(int row, int col);
void generate_terrain(simulation_t *simulation, mesh_table_t *mesh_table);

#endif //GQUEST_GENERATION_H
