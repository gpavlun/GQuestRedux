#include <pthread.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include "game.h"

#include "logging.h"
#include "../main/player_controller.h"
#include "../rendering/render.h"
#include "../tui_panel/terminal_interface.h"
#include "../generation/generation.h"
#include "../main/simulation.h"
#include "../physics/physics.h"

__attribute__((constructor))
static void my_startup(void){  
  setenv("ASAN_OPTIONS", "detect_leaks=0", 0);
  setenv("LSAN_OPTIONS", "detect_leaks=0", 0);
}

void cleanup(engine_t *engine) {
  engine->modes.running = 0;

  if(engine->modes.console){
    pthread_join(engine->console_thread, NULL);
  }
  pthread_join(engine->render_thread, NULL);

  SDL_GL_MakeCurrent(engine->gui.window.interface, NULL);
  SDL_GL_DeleteContext(engine->gl_context);
  SDL_DestroyWindow(engine->gui.window.interface);

  SDL_Quit();

  free(engine->mesh_table.mesh);
  free(engine->texture_table.texture);
  free(engine->shader_table.shader);
  free(engine->simulation.entity_table);
  free(engine->simulation.player_table);
  size_t count = engine->simulation.nterrains;
  for(int i=0; i<count;i++)
    free(engine->simulation.terrain_table[i].heightmap);
  free(engine->simulation.terrain_table);
}

void init_renderer(engine_t *engine) {
  pthread_create(&engine->render_thread, NULL, start_render, engine);
}

void init_console(engine_t *engine) {
  pthread_create(&engine->console_thread, NULL, tui, engine);
}

float cycle_time(u64 *then) {
  u64 now = SDL_GetPerformanceCounter();
  float dt = (float)(now - *then) / (float)SDL_GetPerformanceFrequency();
  *then = now;
  return dt;
}


/***** add_entity *****/
i64 add_entity(simulation_t *simulation, entity_t *entity) {
  if (!simulation) {
    logging.warn("simulation invalid"); return -1;
  }
  if (!entity) {
    logging.warn("entity invalid"); return -1;
  }

  simulation->nentities++;
  entity_t *temp = realloc(simulation->entity_table, sizeof(entity_t) * simulation->nentities);
  if (!temp) logging.error(NO_RETURN, "realloc failed to increase entity table!");
  simulation->entity_table = temp;
  simulation->entity_table[simulation->nentities - 1] = *entity;

  return (i64)simulation->nentities - 1;
}


void create_entities(
  simulation_t *simulation,
  mesh_table_t *mesh_table,
  shader_table_t *shader_table,
  texture_table_t *texture_table
  ) {

  entity_t temp;
  // demo triangle

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $cube_mesh,
      .shader_idx = 0,
      .texture_idx = 4,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = (vec3){ 0 , 0 ,-25},
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ 1 , 1 , 1 }
    }
  };
  temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]) + 3.5f;
  add_entity(simulation, &temp);

  // barad dur
  temp = (entity_t){
    .render_data = {
      .mesh_idx = $barad_dur_mesh,
      .shader_idx = 0,
      .texture_idx = 3,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = (vec3){ 0 , 0 ,-60},
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ 1 , 1 , 1 }
    }
  };
  temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]) - 2.0f;
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $barad_eye_mesh,
      .shader_idx = 0,
      .texture_idx = 4,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = temp.transform.pos,
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ 1 , 1 , 1 }
    }
  };
  temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]) - 2.0f;
  add_entity(simulation, &temp);

  // towers
  temp = (entity_t){
    .render_data = {
      .mesh_idx = $tower_mesh,
      .shader_idx = 0,
      .texture_idx = 1,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = (vec3){-20, 0 ,-20},
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ 1 , 1 , 1 }
    }
  };
  temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]) - 2.0f;
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $tower_roof_mesh,
      .shader_idx = 0,
      .texture_idx = 2,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = temp.transform.pos,
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ 1 , 1 , 1 }
    }
  };
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $tower_mesh,
      .shader_idx = 0,
      .texture_idx = 1,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = (vec3){ 20, 0 ,-20},
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ .5f , .5f , .5f }
    }
  };
  temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]) - 1.0f;
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $tower_roof_mesh,
      .shader_idx = 0,
      .texture_idx = 2,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = temp.transform.pos,
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ .5f , .5f , .5f }
    }
  };
  add_entity(simulation, &temp);

  //castle
  temp = (entity_t){
    .render_data = {
      .mesh_idx = $castle_mesh,
      .shader_idx = 0,
      .texture_idx = 1,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = (vec3){-105,0,+100},
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ 1 , 1 , 1 }
    }
  };
  temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]);
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $castle_roof_mesh,
      .shader_idx = 0,
      .texture_idx = 2,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = temp.transform.pos,
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ 1 , 1 , 1 }
    }
  };
  temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]);
  add_entity(simulation, &temp);

  // trees
  for(int i=0;i<20;i++){
    for(int j=0;j<6;j++){
      temp = (entity_t){
        .render_data = {
          .mesh_idx = $tree_mesh,
          .shader_idx = 0,
          .texture_idx = 5,
          .remodel = 1,
        },
        .physics_actor = {
          0
        },
        .transform = {
          .pos   = (vec3){-35-(5*i)+((rand() % 11) - 5) , 0 , +20+(5*j)+((rand() % 11) - 5)},
          .rot   = (vec3){ 0 , 0 , 0 },
          .scale = (vec3){ .2f , .2f , .2f }
        }
      };
      temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]);
      add_entity(simulation, &temp);
    }
  }
    for(int i=0;i<6;i++){
    for(int j=0;j<20;j++){
      temp = (entity_t){
        .render_data = {
          .mesh_idx = $tree_mesh,
          .shader_idx = 0,
          .texture_idx = 5,
          .remodel = 1,
        },
        .physics_actor = {
          0
        },
        .transform = {
          .pos   = (vec3){-20-(5*i)+((rand() % 11) - 5) , 0 , +65+(5*j)+((rand() % 11) - 5)},
          .rot   = (vec3){ 0 , 0 , 0 },
          .scale = (vec3){ .2f , .2f , .2f }
        }
      };
      temp.transform.pos.y  = terrain_height(temp.transform.pos, &simulation->terrain_table[0]);
      add_entity(simulation, &temp);
    }
  }

}

/***** add_player *****/
i64 add_player(simulation_t *simulation, player_t *player) {
  if (!simulation) {
    logging.warn("simulation invalid"); return -1;
  }
  if (!player) {
    logging.warn("player invalid"); return -1;
  }

  simulation->nplayers++;
  player_t *temp = realloc(simulation->player_table, sizeof(player_t) * simulation->nplayers);
  if (!temp) logging.error(NO_RETURN, "realloc failed to increase player table!");
  simulation->player_table = temp;
  simulation->player_table[simulation->nplayers - 1] = *player;

  return (i64)simulation->nplayers - 1;
}

void create_player(simulation_t *simulation) {
  player_t temp;
  // player
  temp = (player_t){
    .entity = {
      .transform = {
        .pos   = (vec3){ 0 , 0 , 0 },
        .rot   = (vec3){ 0 , 0 , 0 },
        .scale = (vec3){ 1 , 1 , 1 }
      },
      .physics_actor = {
        .mass = 80,
        .max_force = 2470,
        .walkspeed = 6,
        .strafespeed = 6/1.5
      },
      .render_data = {0}
    },
    .camera = {
      .pos        = (vec3){ 0 , 0 , 0 },
      .rot        = (vec3){ 0 , 0 , 0 },
      .offset     = (vec3){ 0 , 2 , 0 },
      .projection = {0},
      .view       = {0}
    },
    .flying = 0,
    .perspective = 0
  };
  temp.entity.transform.pos.y  = 8.0f;//terrain_height(temp.entity.transform.pos, &simulation->terrain_table[0]) + 1.0f;
  temp.camera.pos = temp.entity.transform.pos;
  add_player(simulation, &temp);
}



int main(int argc, char **argv){

  engine_t engine = {0};

  engine.gui.window.dim.w = 800;
  engine.gui.window.dim.h = 600;

  if(argc > 1){
    for(int a = 1; a < argc; a++){
      if(!strcmp(argv[a], "--width") || !strcmp(argv[a], "-ww")){
        if(a + 1 < argc){
          char *end;
          errno = 0;
          unsigned long val = strtoul(argv[++a], &end, 10);

          if(errno == 0 && *end == '\0' && val > 0 && val <= SIZE_MAX){
            engine.gui.window.dim.w = (size_t)val;
          }
        }
      }else if(!strcmp(argv[a], "--height") || !strcmp(argv[a], "-wh")){
        if(a + 1 < argc){
          char *end;
          errno = 0;
          unsigned long val = strtoul(argv[++a], &end, 10);

          if(errno == 0 && *end == '\0' && val > 0 && val <= SIZE_MAX){
            engine.gui.window.dim.h  = (size_t)val;
          }
        }
      }else if(!strcmp(argv[a], "--console") || !strcmp(argv[a], "-c")){
        engine.modes.console=true;
      }else if(!strcmp(argv[a], "--logpath") || !strcmp(argv[a], "-lp")){
        if(a + 1 < argc){
          logging.path(argv[a + 1]);
        }
      }else if(!strcmp(argv[a], "--help") || !strcmp(argv[a], "-h")){
        system("cat ./docs/cli_args.txt");
        exit(0);
      }
    }
  }


  
  pthread_barrier_init(&engine.barrier, NULL, 2 + engine.modes.console);

  logging.info("initializing windowing...");
  init_sdl_engine(&engine.gui);
  logging.info("windowing initialized");

  logging.info("generating meshes...");
  generate_meshes(&engine.mesh_table);
  logging.info("meshes generated");
  logging.data("mesh count:", engine.mesh_table.nmeshes);

  logging.info("generating shaders...");
  generate_shaders(&engine.shader_table);
  logging.info("shaders generated");
  logging.data("shader count:", engine.shader_table.nshaders);

  logging.info("generating textures...");
  generate_textures(&engine.texture_table);
  logging.info("textures generated");
  logging.data("texture count:", engine.texture_table.ntextures);

  logging.info("generating terrain...");
  generate_terrain(&engine.simulation, &engine.mesh_table);
  logging.info("terrain generated");

  logging.info("creating entities...");
  create_entities(&engine.simulation, &engine.mesh_table, &engine.shader_table, &engine.texture_table);
  logging.info("entities created");

  logging.info("creating players...");
  create_player(&engine.simulation);
  logging.info("player created");

  logging.info("starting render thread");
  init_renderer(&engine);
  logging.info("render thread started");

  if(engine.modes.console){
    logging.info("starting console thread");
    init_console(&engine);
    logging.info("console thread started");
  }


  logging.info("initializing controller");
  init_controller(&engine.controller);
  logging.info("controller initialized");

  /*
   * basically, the uncomposed systems fail because there
   * is no orchestrating simulation code. I believe this
   * is corrected with the new model. This layout is
   * funadmentally correct now, it will just need some
   * tweaks to make it repeatable for different game
   * states.
   */

  u64 event_cycle = SDL_GetPerformanceCounter();

  engine.modes.running = true;
  logging.info("main waiting at barrier");
  pthread_barrier_wait(&engine.barrier);
  logging.info("=== STARTING ENGINE LOOP ===");
  while (engine.modes.running) {

    engine.controller.dt = cycle_time(&event_cycle);
    event_handler(&engine.controller, &engine.simulation, &engine.modes);
    step_simulation(&engine.simulation);
    step_physics(&engine.controller, &engine.simulation);

  }
  logging.info("=== ENGINE LOOP FINISHED ===");
  cleanup(&engine);
  return 0;
}




