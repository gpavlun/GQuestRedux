#include "gio.h"
#include "../rendering/render.h"
#include "../rendering/obj_parser.h"
#include "../main/game.h"

/***** add_mesh *****/
i64 add_mesh(mesh_table_t *mesh_table, mesh_t *mesh) {
  if (!mesh_table) {
    logging.warn("mesh table invalid"); return -1;
  }
  if (!mesh) {
    logging.warn("mesh invalid"); return -1;
  }

  mesh_table->nmeshes++;
  mesh_t *temp = realloc(mesh_table->mesh, sizeof(mesh_t) * mesh_table->nmeshes);
  if (!temp) logging.error(NO_RETURN, "realloc failed to increase mesh table!");
  mesh_table->mesh = temp;
  mesh_table->mesh[mesh_table->nmeshes - 1] = *mesh;

  return (i64)mesh_table->nmeshes - 1;
}

/***** mesh_generate_normals *****/
void mesh_generate_normals(mesh_t *mesh){
  if(!mesh) return;

  u32 i;

  for(i=0;i<mesh->nverts;i++){
    mesh->vert[i].normal = (vec3){0,0,0};
  }

  for(i=0;i<(mesh->ntris);i++){

    u32 ia = mesh->tri[i].a;
    u32 ib = mesh->tri[i].b;
    u32 ic = mesh->tri[i].c;

    vec3 a = mesh->vert[ia].pos;
    vec3 b = mesh->vert[ib].pos;
    vec3 c = mesh->vert[ic].pos;

    vec3 edge1 = vec3_sub(b,a);
    vec3 edge2 = vec3_sub(c,a);

    vec3 normal = vec3_cross(edge1,edge2);
    normal = vec3_normalize(normal);

    mesh->vert[ia].normal =
      vec3_add(mesh->vert[ia].normal,normal);

    mesh->vert[ib].normal =
      vec3_add(mesh->vert[ib].normal,normal);

    mesh->vert[ic].normal =
      vec3_add(mesh->vert[ic].normal,normal);
  }

  for(i=0;i<mesh->nverts;i++){
    mesh->vert[i].normal =
      vec3_normalize(mesh->vert[i].normal);
  }
}

/***** new_mesh *****/
mesh_t new_mesh(size_t nverts, vertex_t *verts, size_t ntris, tri_t *tris) {
  mesh_t mesh = {0};

  mesh.nverts = nverts;
  mesh.vert   = verts;
  mesh.ntris  = ntris;
  mesh.tri    = tris;

  return mesh;
}
void mesh_generate_uv(mesh_t *mesh)
{
    float min_x = FLT_MAX;
    float max_x = -FLT_MAX;
    float min_y = FLT_MAX;
    float max_y = -FLT_MAX;

    for (int i = 0; i < mesh->nverts; i++) {
        vec3 p = mesh->vert[i].pos;

        min_x = fminf(min_x, p.x);
        max_x = fmaxf(max_x, p.x);

        min_y = fminf(min_y, p.y);
        max_y = fmaxf(max_y, p.y);
    }

    float sx = max_x - min_x;
    float sy = max_y - min_y;

    for (int i = 0; i < mesh->nverts; i++) {
        mesh->vert[i].uv.x = (mesh->vert[i].pos.x - min_x) / sx;
        mesh->vert[i].uv.y = (mesh->vert[i].pos.y - min_y) / sy;
    }
}
/***** generate_meshes *****/
void generate_meshes(mesh_table_t *mesh_table) {
  vertex_t *vertices = NULL;
  tri_t *triangles = NULL;
  mesh_t temp_mesh;

  // eventually replace this with a lookup mechanism
  // and remove harcoded meshes

  // triangle
  demo_tri_;
  temp_mesh = new_mesh(3, vertices, 1, triangles);
  mesh_generate_normals(&temp_mesh);
  mesh_generate_uv(&temp_mesh);
  add_mesh(mesh_table, &temp_mesh);

  // barad dur
  temp_mesh = obj_from_file("./assets/obj_models/barad_dur.obj");
  mesh_generate_normals(&temp_mesh);
  mesh_generate_uv(&temp_mesh);
  add_mesh(mesh_table, &temp_mesh);

  // barad dur eye
  temp_mesh = obj_from_file("./assets/obj_models/eye.obj");
  mesh_generate_normals(&temp_mesh);
  mesh_generate_uv(&temp_mesh);
  add_mesh(mesh_table, &temp_mesh);

  // player
  temp_mesh = obj_from_file("./assets/obj_models/player.obj");
  mesh_generate_normals(&temp_mesh);
  mesh_generate_uv(&temp_mesh);
  add_mesh(mesh_table, &temp_mesh);

  // lone tower
  temp_mesh = obj_from_file("./assets/obj_models/tower_body.obj");
  mesh_generate_normals(&temp_mesh);
  mesh_generate_uv(&temp_mesh);
  add_mesh(mesh_table, &temp_mesh);

  // lone tower roof
  temp_mesh = obj_from_file("./assets/obj_models/tower_roof.obj");
  mesh_generate_normals(&temp_mesh);
  mesh_generate_uv(&temp_mesh);
  add_mesh(mesh_table, &temp_mesh);

  // castle
  temp_mesh = obj_from_file("./assets/obj_models/castle.obj");
  mesh_generate_normals(&temp_mesh);
  mesh_generate_uv(&temp_mesh);
  add_mesh(mesh_table, &temp_mesh);

  // castle roof
  temp_mesh = obj_from_file("./assets/obj_models/roofs.obj");
  mesh_generate_normals(&temp_mesh);
  mesh_generate_uv(&temp_mesh);
  add_mesh(mesh_table, &temp_mesh);

}