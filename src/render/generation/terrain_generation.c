#include "generation.h"
#include "logging.h"

#include "../rendering/render.h"
#include "../main/game.h"

float height_func(int row, int col){
  float y = (float)row;
  float x = (float)col;
  float h = 0.0f;
  // Large rolling terrain
  h += sinf(x * 0.025f + y * 0.015f) * 12.0f;
  h += cosf(y * 0.035f - x * 0.020f) * 8.0f;
  // Medium features
  h += sinf(x * 0.08f) * cosf(y * 0.06f) * 4.0f;
  // Small roughness
  h += sinf(x * 0.25f + y * 0.15f) * 1.5f;
  h += cosf(x * 0.40f - y * 0.30f) * 0.5f;
  return h;
}

void generate_terrain(simulation_t *simulation, mesh_table_t *mesh_table){
  i32 r, c, t;

  logical_chunk_t *logical_chunk = calloc(1, sizeof(logical_chunk_t));
  logical_chunk->heightmap = calloc($chunk_verts*$chunk_verts, sizeof(float));
  for(r=0; r<$chunk_verts; r++){
    for(c=0; c<$chunk_verts; c++){
      *(logical_chunk->heightmap + r*$chunk_verts + c) =
      height_func(r - $chunk_verts/2, c - $chunk_verts/2);
    }
  }
  // flat spot
#define $flat_w 85
  for(r=0;r<$flat_w;r++){
    for(c=0;c<$flat_w;c++){
      *(logical_chunk->heightmap +
      ($chunk_verts/2+$flat_w/2+100-r)*$chunk_verts +
      ($chunk_verts/2+$flat_w/2-100-c)) = 0.0f;
    }
  }
  simulation->terrain_table = logical_chunk;
  simulation->nterrains = 1;

  vertex_t *vertices = NULL;
  tri_t *triangles = NULL;

  vertices  = calloc($chunk_verts * $chunk_verts, sizeof(vertex_t));
  triangles = calloc($chunk_quads * $chunk_quads* 2, sizeof(tri_t));

  for(r=0; r<$chunk_verts; r++){
    for(c=0; c<$chunk_verts; c++){
      (vertices + r*$chunk_verts + c)->pos =
        (vec3){
          (float)c - (float)$chunk_verts/2,
          logical_chunk->heightmap[r*$chunk_verts+c],
          (float)r - (float)$chunk_verts/2
        };

      (vertices + r*$chunk_verts + c)->uv = (vec2){0.33f, 0.7f};
    }
  }
  t=0;
  for(r = 0; r < $chunk_quads; r++){
    for(c = 0; c < $chunk_quads; c++){

      u32 a = r * $chunk_verts + c;
      u32 b = r * $chunk_verts + c + 1;
      u32 c_idx = (r + 1) * $chunk_verts + c;
      u32 d = (r + 1) * $chunk_verts + c + 1;

      triangles[t++] = (tri_t){a, c_idx, b};
      triangles[t++] = (tri_t){b, c_idx, d};
    }
  }


  logging.assert(t == $chunk_quads * $chunk_quads * 2,
                 "terrain failure, quads misaligned!");

  mesh_t temp_mesh = new_mesh($chunk_verts*$chunk_verts, vertices,
                              $chunk_quads*$chunk_quads*2, triangles);
  mesh_generate_normals(&temp_mesh);
  simulation->terrain_table[0].mesh_idx = add_mesh(mesh_table, &temp_mesh);
  simulation->terrain_table[0].shader_idx = 0;
  simulation->terrain_table[0].texture_idx = 0;
}