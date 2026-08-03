//
// Created by berserker on 8/2/26.
//

#ifndef GQUEST_GENERATION_H
#define GQUEST_GENERATION_H

#include "../rendering/render.h"

void generate_shaders(shader_table_t *shader_table);
i64 add_shader(shader_table_t *shader_table, shader_t *shader);

void generate_meshes(mesh_table_t *mesh_table);
void mesh_generate_normals(mesh_t *mesh);
mesh_t new_mesh(size_t nverts, vertex_t *verts, size_t ntris, tri_t *tris);
i64 add_mesh(mesh_table_t *mesh_table, mesh_t *mesh);

float height_func(int row, int col);
void generate_terrain(simulation_t *simulation, mesh_table_t *mesh_table);

#endif //GQUEST_GENERATION_H
