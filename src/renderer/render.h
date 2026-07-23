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
  ground.trangs = malloc(sizeof(triangle_t) * ground.ntrangs);\
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



#define tower_                                          \
  mesh_t tower = {0};                                   \
  tower.nverts = 33;                                     \
  tower.verts = malloc(sizeof(vert_t) * tower.nverts);  \
                                                        \
  tower.verts[0].x = -18;                               \
  tower.verts[0].y = 0;                                 \
  tower.verts[0].z = -25;                               \
                                                        \
  tower.verts[1].x = -22;                               \
  tower.verts[1].y = 0;                                 \
  tower.verts[1].z = -25;                               \
                                                        \
  tower.verts[2].x = -25;                               \
  tower.verts[2].y = 0;                                 \
  tower.verts[2].z = -22;                               \
                                                        \
  tower.verts[3].x = -25;                               \
  tower.verts[3].y = 0;                                 \
  tower.verts[3].z = -18;                               \
                                                        \
  tower.verts[4].x = -22;                               \
  tower.verts[4].y = 0;                                 \
  tower.verts[4].z = -15;                               \
                                                        \
  tower.verts[5].x = -18;                               \
  tower.verts[5].y = 0;                                 \
  tower.verts[5].z = -15;                               \
                                                        \
  tower.verts[6].x = -15;                               \
  tower.verts[6].y = 0;                                 \
  tower.verts[6].z = -18;                               \
                                                        \
  tower.verts[7].x = -15;                               \
  tower.verts[7].y = 0;                                 \
  tower.verts[7].z = -22;                               \
                                                        \
  tower.verts[8].x = -18;                               \
  tower.verts[8].y = 15;                                \
  tower.verts[8].z = -25;                               \
                                                        \
  tower.verts[9].x = -22;                               \
  tower.verts[9].y = 15;                                \
  tower.verts[9].z = -25;                               \
                                                        \
  tower.verts[10].x = -25;                              \
  tower.verts[10].y = 15;                               \
  tower.verts[10].z = -22;                              \
                                                        \
  tower.verts[11].x = -25;                              \
  tower.verts[11].y = 15;                               \
  tower.verts[11].z = -18;                              \
                                                        \
  tower.verts[12].x = -22;                              \
  tower.verts[12].y = 15;                               \
  tower.verts[12].z = -15;                              \
                                                        \
  tower.verts[13].x = -18;                              \
  tower.verts[13].y = 15;                               \
  tower.verts[13].z = -15;                              \
                                                        \
  tower.verts[14].x = -15;                              \
  tower.verts[14].y = 15;                               \
  tower.verts[14].z = -18;                              \
                                                        \
  tower.verts[15].x = -15;                              \
  tower.verts[15].y = 15;                               \
  tower.verts[15].z = -22;                              \
                                                        \
  tower.verts[16].x = -18;                               \
  tower.verts[16].y = 15;                                \
  tower.verts[16].z = -26;                               \
                                                        \
  tower.verts[17].x = -22;                               \
  tower.verts[17].y = 15;                                \
  tower.verts[17].z = -26;                               \
                                                        \
  tower.verts[18].x = -26;                              \
  tower.verts[18].y = 15;                               \
  tower.verts[18].z = -22;                              \
                                                        \
  tower.verts[19].x = -26;                              \
  tower.verts[19].y = 15;                               \
  tower.verts[19].z = -18;                              \
                                                        \
  tower.verts[20].x = -22;                              \
  tower.verts[20].y = 15;                               \
  tower.verts[20].z = -14;                              \
                                                        \
  tower.verts[21].x = -18;                              \
  tower.verts[21].y = 15;                               \
  tower.verts[21].z = -14;                              \
                                                        \
  tower.verts[22].x = -14;                              \
  tower.verts[22].y = 15;                               \
  tower.verts[22].z = -18;                              \
                                                        \
  tower.verts[23].x = -14;                              \
  tower.verts[23].y = 15;                               \
  tower.verts[23].z = -22;                              \
                                                        \
  tower.verts[24].x = -18;                               \
  tower.verts[24].y = 16;                                \
  tower.verts[24].z = -26;                               \
                                                        \
  tower.verts[25].x = -22;                               \
  tower.verts[25].y = 16;                                \
  tower.verts[25].z = -26;                               \
                                                        \
  tower.verts[26].x = -26;                              \
  tower.verts[26].y = 16;                               \
  tower.verts[26].z = -22;                              \
                                                        \
  tower.verts[27].x = -26;                              \
  tower.verts[27].y = 16;                               \
  tower.verts[27].z = -18;                              \
                                                        \
  tower.verts[28].x = -22;                              \
  tower.verts[28].y = 16;                               \
  tower.verts[28].z = -14;                              \
                                                        \
  tower.verts[29].x = -18;                              \
  tower.verts[29].y = 16;                               \
  tower.verts[29].z = -14;                              \
                                                        \
  tower.verts[30].x = -14;                              \
  tower.verts[30].y = 16;                               \
  tower.verts[30].z = -18;                              \
                                                        \
  tower.verts[31].x = -14;                              \
  tower.verts[31].y = 16;                               \
  tower.verts[31].z = -22;                              \
                                                        \
  tower.verts[32].x = -20;                              \
  tower.verts[32].y = 25;                               \
  tower.verts[32].z = -20;                              \
                                                        \
  tower.ntrangs = 56;                                   \
  tower.trangs = malloc(sizeof(triangle_t)*tower.ntrangs);\
  int iki = 0;\
                                                        \
  tower.trangs[iki].a = 0;                               \
  tower.trangs[iki].b = 1;                               \
  tower.trangs[iki].c = 9;                               \
  iki++;                                                      \
  tower.trangs[1].a = 1;                               \
  tower.trangs[1].b = 2;                               \
  tower.trangs[1].c = 10;                              \
  iki++;                                                       \
  tower.trangs[2].a = 2;                               \
  tower.trangs[2].b = 3;                               \
  tower.trangs[2].c = 11;                              \
  iki++;                                                       \
  tower.trangs[3].a = 3;                               \
  tower.trangs[3].b = 4;                               \
  tower.trangs[3].c = 12;                              \
  iki++;                                                       \
  tower.trangs[4].a = 4;                               \
  tower.trangs[4].b = 5;                               \
  tower.trangs[4].c = 13;                              \
  iki++;                                                       \
  tower.trangs[5].a = 5;                               \
  tower.trangs[5].b = 6;                               \
  tower.trangs[5].c = 14;                              \
  iki++;                                                       \
  tower.trangs[6].a = 6;                               \
  tower.trangs[6].b = 7;                               \
  tower.trangs[6].c = 15;                              \
  iki++;                                                       \
  tower.trangs[7].a = 7;                               \
  tower.trangs[7].b = 0;                               \
  tower.trangs[7].c = 8;                               \
  iki++;                                                     \
  tower.trangs[8].a = 9;                               \
  tower.trangs[8].b = 8;                               \
  tower.trangs[8].c = 0;                               \
  iki++;                                                       \
  tower.trangs[9].a = 10;                               \
  tower.trangs[9].b = 9;                               \
  tower.trangs[9].c = 1;                              \
  iki++;                                                       \
  tower.trangs[10].a = 11;                               \
  tower.trangs[10].b = 10;                               \
  tower.trangs[10].c = 2;                              \
  iki++;                                                       \
  tower.trangs[11].a = 12;                               \
  tower.trangs[11].b = 11;                               \
  tower.trangs[11].c = 3;                              \
  iki++;                                                       \
  tower.trangs[12].a = 13;                               \
  tower.trangs[12].b = 12;                               \
  tower.trangs[12].c = 4;                              \
  iki++;                                                       \
  tower.trangs[13].a = 14;                               \
  tower.trangs[13].b = 13;                               \
  tower.trangs[13].c = 5;                              \
  iki++;                                                       \
  tower.trangs[14].a = 15;                               \
  tower.trangs[14].b = 14;                               \
  tower.trangs[14].c = 6;                              \
  iki++;                                                       \
  tower.trangs[15].a = 8;                               \
  tower.trangs[15].b = 15;                               \
  tower.trangs[15].c = 7;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 17;                               \
  tower.trangs[iki].b = 9;                               \
  tower.trangs[iki].c = 8;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 18;                               \
  tower.trangs[iki].b = 10;                               \
  tower.trangs[iki].c = 9;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 19;                               \
  tower.trangs[iki].b = 11;                               \
  tower.trangs[iki].c = 10;                               \
  iki++; \
  tower.trangs[iki].a = 20;                               \
  tower.trangs[iki].b = 12;                               \
  tower.trangs[iki].c = 11;                               \
  iki++; \
  tower.trangs[iki].a = 21;                               \
  tower.trangs[iki].b = 13;                               \
  tower.trangs[iki].c = 12;                               \
  iki++; \
  tower.trangs[iki].a = 22;                               \
  tower.trangs[iki].b = 14;                               \
  tower.trangs[iki].c = 13;                               \
  iki++; \
  tower.trangs[iki].a = 23;                               \
  tower.trangs[iki].b = 15;                               \
  tower.trangs[iki].c = 14;                               \
  iki++; \
  tower.trangs[iki].a = 16;                               \
  tower.trangs[iki].b = 8;                               \
  tower.trangs[iki].c = 15;                               \
  iki++;                                                  \
  tower.trangs[iki].a = 17;                               \
  tower.trangs[iki].b = 8;                               \
  tower.trangs[iki].c = 16;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 18;                               \
  tower.trangs[iki].b = 9;                               \
  tower.trangs[iki].c = 17;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 19;                               \
  tower.trangs[iki].b = 10;                               \
  tower.trangs[iki].c = 18;                               \
  iki++; \
  tower.trangs[iki].a = 20;                               \
  tower.trangs[iki].b = 11;                               \
  tower.trangs[iki].c = 19;                               \
  iki++; \
  tower.trangs[iki].a = 21;                               \
  tower.trangs[iki].b = 12;                               \
  tower.trangs[iki].c = 20;                               \
  iki++; \
  tower.trangs[iki].a = 22;                               \
  tower.trangs[iki].b = 13;                               \
  tower.trangs[iki].c = 21;                               \
  iki++; \
  tower.trangs[iki].a = 23;                               \
  tower.trangs[iki].b = 14;                               \
  tower.trangs[iki].c = 22;                               \
  iki++; \
  tower.trangs[iki].a = 16;                               \
  tower.trangs[iki].b = 15;                               \
  tower.trangs[iki].c = 23;                               \
  iki++;                                                  \
  tower.trangs[iki].a = 17;                               \
  tower.trangs[iki].b = 25;                               \
  tower.trangs[iki].c = 16;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 18;                               \
  tower.trangs[iki].b = 26;                               \
  tower.trangs[iki].c = 17;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 19;                               \
  tower.trangs[iki].b = 27;                               \
  tower.trangs[iki].c = 18;                               \
  iki++; \
  tower.trangs[iki].a = 20;                               \
  tower.trangs[iki].b = 28;                               \
  tower.trangs[iki].c = 19;                               \
  iki++; \
  tower.trangs[iki].a = 21;                               \
  tower.trangs[iki].b = 29;                               \
  tower.trangs[iki].c = 20;                               \
  iki++; \
  tower.trangs[iki].a = 22;                               \
  tower.trangs[iki].b = 30;                               \
  tower.trangs[iki].c = 21;                               \
  iki++; \
  tower.trangs[iki].a = 23;                               \
  tower.trangs[iki].b = 31;                               \
  tower.trangs[iki].c = 22;                               \
  iki++; \
  tower.trangs[iki].a = 16;                               \
  tower.trangs[iki].b = 24;                               \
  tower.trangs[iki].c = 23;                               \
  iki++;\
  tower.trangs[iki].a = 25;                               \
  tower.trangs[iki].b = 24;                               \
  tower.trangs[iki].c = 16;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 26;                               \
  tower.trangs[iki].b = 25;                               \
  tower.trangs[iki].c = 17;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 27;                               \
  tower.trangs[iki].b = 26;                               \
  tower.trangs[iki].c = 18;                               \
  iki++; \
  tower.trangs[iki].a = 28;                               \
  tower.trangs[iki].b = 27;                               \
  tower.trangs[iki].c = 19;                               \
  iki++; \
  tower.trangs[iki].a = 29;                               \
  tower.trangs[iki].b = 28;                               \
  tower.trangs[iki].c = 20;                               \
  iki++; \
  tower.trangs[iki].a = 30;                               \
  tower.trangs[iki].b = 29;                               \
  tower.trangs[iki].c = 21;                               \
  iki++; \
  tower.trangs[iki].a = 31;                               \
  tower.trangs[iki].b = 30;                               \
  tower.trangs[iki].c = 22;                               \
  iki++; \
  tower.trangs[iki].a = 24;                               \
  tower.trangs[iki].b = 31;                               \
  tower.trangs[iki].c = 23;                               \
  iki++;\
  tower.trangs[iki].a = 8;                               \
  tower.trangs[iki].b = 9;                               \
  tower.trangs[iki].c = 32;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 9;                               \
  tower.trangs[iki].b = 10;                               \
  tower.trangs[iki].c = 32;                               \
  iki++;                                                       \
  tower.trangs[iki].a = 10;                               \
  tower.trangs[iki].b = 11;                               \
  tower.trangs[iki].c = 32;                               \
  iki++; \
  tower.trangs[iki].a = 11;                               \
  tower.trangs[iki].b = 12;                               \
  tower.trangs[iki].c = 32;                               \
  iki++; \
  tower.trangs[iki].a = 12;                               \
  tower.trangs[iki].b = 13;                               \
  tower.trangs[iki].c = 32;                               \
  iki++; \
  tower.trangs[iki].a = 13;                               \
  tower.trangs[iki].b = 14;                               \
  tower.trangs[iki].c = 32;                               \
  iki++; \
  tower.trangs[iki].a = 14;                               \
  tower.trangs[iki].b = 15;                               \
  tower.trangs[iki].c = 32;                               \
  iki++; \
  tower.trangs[iki].a = 15;                               \
  tower.trangs[iki].b = 8;                               \
  tower.trangs[iki].c = 32;                               \
  iki++;\
  tower.opts.triangles = 1;                             \
  tower.opts.wire_frame = 1;                             \
  tower.opts.vertices = 1;                             \
  tower.color.code = 0x3E3838;





#endif