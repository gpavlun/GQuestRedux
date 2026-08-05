//
// Created by berserker on 7/31/26.
//

#include "obj_parser.h"
#include "../generation/generation.h"

mesh_t obj_from_file(char *filename) {

  size_t vertex_count = 0;
  size_t triangle_count = 0;

  FILE *fp = fopen(filename, "r");
  if (!fp){
    logging.warn("obj file not found");
    goto missing;
  }

  char line[512];

  // count vertices and faces
  while (fgets(line, sizeof(line), fp)) {
    if (strncmp(line, "v ", 2) == 0)
      vertex_count++;

    else if (strncmp(line, "f ", 2) == 0)
      triangle_count++;
  }

  vertex_t *vertices = calloc(vertex_count, sizeof(vertex_t));
  tri_t    *triangles = calloc(triangle_count, sizeof(tri_t));

  rewind(fp);

  size_t current_vertex = 0;
  size_t current_triangle = 0;

  // get all vertices
  while (fgets(line, sizeof(line), fp)) {
    if (strncmp(line, "v ", 2) == 0) {
      float x, y, z;

      sscanf(line, "v %f %f %f", &x, &z, &y);

      vertices[current_vertex++] = (vertex_t){
        .pos = {x, y, z},
        .normal = {0,0,0},
        .uv = {0,0}
      };
    }else if (strncmp(line, "f ", 2) == 0){
      unsigned a, b, c;

      sscanf(line, "f %u %u %u", &c, &b, &a);

      triangles[current_triangle++] = (tri_t){a - 1, b - 1, c - 1};
    }
  }
  fclose(fp);

  mesh_t mesh = new_mesh(
      vertex_count,
      vertices,
      triangle_count,
      triangles
  );

  return mesh;

  missing:
  return obj_from_file("./assets/obj_models/placeholder.obj");

}