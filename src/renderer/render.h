#ifndef RENDER_H
#define RENDER_H

#define cubiod_                                         \
mesh_t cuboid = {0};                                    \
  cuboid.nverts = 8;                                    \
  cuboid.verts = malloc(sizeof(vert_t) * cuboid.nverts);\
                                                        \
  cuboid.verts[0].x = 0;                                \
  cuboid.verts[0].y = 0;                                \
  cuboid.verts[0].z = 30;                               \
                                                        \
  cuboid.verts[1].x = 0;                                \
  cuboid.verts[1].y = 0;                                \
  cuboid.verts[1].z = 35;                               \
                                                        \
  cuboid.verts[2].x = 5;                                \
  cuboid.verts[2].y = 0;                                \
  cuboid.verts[2].z = 35;                               \
                                                        \
  cuboid.verts[3].x = 5;                                \
  cuboid.verts[3].y = 0;                                \
  cuboid.verts[3].z = 30;                               \
                                                        \
  cuboid.verts[4].x = 0;                                \
  cuboid.verts[4].y = 5;                                \
  cuboid.verts[4].z = 30;                               \
                                                        \
  cuboid.verts[5].x = 0;                                \
  cuboid.verts[5].y = 5;                                \
  cuboid.verts[5].z = 35;                               \
                                                        \
  cuboid.verts[6].x = 5;                                \
  cuboid.verts[6].y = 5;                                \
  cuboid.verts[6].z = 35;                               \
                                                        \
  cuboid.verts[7].x = 5;                                \
  cuboid.verts[7].y = 5;                                \
  cuboid.verts[7].z = 30;                               \
                                                        \
  cuboid.nedges = 12;                                   \
  cuboid.edges = malloc(sizeof(edge_t) * cuboid.nedges);\
                                                        \
  cuboid.edges[0].A = 0;                                \
  cuboid.edges[0].B = 1;                                \
                                                        \
  cuboid.edges[1].A = 1;                                \
  cuboid.edges[1].B = 2;                                \
                                                        \
  cuboid.edges[2].A = 2;                                \
  cuboid.edges[2].B = 3;                                \
                                                        \
  cuboid.edges[3].A = 3;                                \
  cuboid.edges[3].B = 0;                                \
                                                        \
  cuboid.edges[4].A = 4;                                \
  cuboid.edges[4].B = 5;                                \
                                                        \
  cuboid.edges[5].A = 5;                                \
  cuboid.edges[5].B = 6;                                \
                                                        \
  cuboid.edges[6].A = 6;                                \
  cuboid.edges[6].B = 7;                                \
                                                        \
  cuboid.edges[7].A = 7;                                \
  cuboid.edges[7].B = 4;                                \
                                                        \
  cuboid.edges[8].A = 0;                                \
  cuboid.edges[8].B = 4;                                \
                                                        \
  cuboid.edges[9].A = 1;                                \
  cuboid.edges[9].B = 5;                                \
                                                        \
  cuboid.edges[10].A = 2;                               \
  cuboid.edges[10].B = 6;                               \
                                                        \
  cuboid.edges[11].A = 3;                               \
  cuboid.edges[11].B = 7;                               \
                                                        \
  cuboid.opts.wire_frame = 1;                           \
  cuboid.opts.vertices = 1;                             \
  cuboid.color.code = 0xFF0000;                     








  
  #define ground_                                       \
  mesh_t ground = {0};                                  \
  ground.nverts = 4;                                    \
  ground.verts = malloc(sizeof(vert_t) * ground.nverts);\
                                                        \
  ground.verts[0].x = -100;                             \
  ground.verts[0].y = 0;                                \
  ground.verts[0].z = -100;                             \
                                                        \
  ground.verts[1].x = -100;                             \
  ground.verts[1].y = 0;                                \
  ground.verts[1].z = 100;                              \
                                                        \
  ground.verts[2].x = 100;                              \
  ground.verts[2].y = 0;                                \
  ground.verts[2].z = -100;                             \
                                                        \
  ground.verts[3].x = 100;                              \
  ground.verts[3].y = 0;                                \
  ground.verts[3].z = 100;                              \
                                                        \
  ground.nedges = 4;                                    \
  ground.edges = malloc(sizeof(edge_t) * ground.nedges);\
                                                        \
  ground.edges[0].A = 0;                                \
  ground.edges[0].B = 1;                                \
                                                        \
  ground.edges[1].A = 1;                                \
  ground.edges[1].B = 2;                                \
                                                        \
  ground.edges[2].A = 2;                                \
  ground.edges[2].B = 3;                                \
                                                        \
  ground.edges[3].A = 3;                                \
  ground.edges[3].B = 0;                                \
                                                        \
  ground.ntrangs = 2;                                   \
  ground.trangs = malloc(sizeof(triangle_t) * ground.nedges);\
                                                        \
  ground.trangs[0].a = 0;                               \
  ground.trangs[0].b = 1;                               \
  ground.trangs[0].c = 2;                               \
                                                        \
  ground.trangs[1].a = 1;                               \
  ground.trangs[1].b = 3;                               \
  ground.trangs[1].c = 2;                               \
                                                        \
  ground.opts.triangles = 1;                            \
  ground.color.code = 0x002D04;















#endif