#include <pthread.h>

#include "game.h"

#include "player_controller.h"
#include "../rendering/render.h"
#include "../tui_panel/terminal_interface.h"
#include "../generation/generation.h"


void cleanup(engine_t *engine) {
  engine->modes.running = 0;
  pthread_join(engine->console_thread, NULL);
  pthread_join(engine->render_thread, NULL);
}

void init_renderer(engine_t *engine) {
  //simulation_t *simulation, mesh_t *mesh_table, pthread_barrier_t *barrier
  pthread_create(&engine->render_thread, NULL, start_render, &engine);
}

void init_console(engine_t *engine) {
  pthread_create(&engine->console_thread, NULL, tui, &engine);
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
  shader_table_t *shader_table
  ) {

  entity_t temp;
  // demo triangle
  temp = (entity_t){
    .render_data = {
      .mesh_idx = $demo_tri_mesh,
      .shader_idx = 0,
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
  temp.transform.pos.y  = terrain_height(temp.transform.pos) + 1.0f;
  add_entity(simulation, &temp);

  // barad dur
  temp = (entity_t){
    .render_data = {
      .mesh_idx = $barad_dur_mesh,
      .shader_idx = 0,
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
  temp.transform.pos.y  = terrain_height(temp.transform.pos) - 2.0f;
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $barad_eye_mesh,
      .shader_idx = 0,
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
  temp.transform.pos.y  = terrain_height(temp.transform.pos) - 2.0f;
  add_entity(simulation, &temp);

  // towers
  temp = (entity_t){
    .render_data = {
      .mesh_idx = $tower_mesh,
      .shader_idx = 0,
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
  temp.transform.pos.y  = terrain_height(temp.transform.pos) - 2.0f;
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $tower_roof_mesh,
      .shader_idx = 0,
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
  temp.transform.pos.y  = terrain_height(temp.transform.pos) - 1.0f;
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $tower_roof_mesh,
      .shader_idx = 0,
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

  //castle
  temp = (entity_t){
    .render_data = {
      .mesh_idx = $castle_mesh,
      .shader_idx = 0,
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
  temp.transform.pos.y  = terrain_height(temp.transform.pos);
  add_entity(simulation, &temp);

  temp = (entity_t){
    .render_data = {
      .mesh_idx = $castle_roof_mesh,
      .shader_idx = 0,
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
  temp.transform.pos.y  = terrain_height(temp.transform.pos);
  add_entity(simulation, &temp);
}

/***** add_player *****/
i64 add_player(simulation_t *simulation, player_t *player) {
  if (!simulation) {
    logging.warn("simulation invalid"); return -1;
  }
  if (!player) {
    logging.warn("player invalid"); return -1;
  }

  simulation->nentities++;
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
    .render_data = {
      .mesh_idx = $player_mesh,
      .shader_idx = 0,
      .remodel = 1,
    },
    .physics_actor = {
      0
    },
    .transform = {
      .pos = (vec3){0,0,0},
      .rot = (vec3){ 0 , 0 , 0 },
      .scale = (vec3){ 1 , 1 , 1 }
    }
  };
  temp.transform.pos.y  = terrain_height(temp.transform.pos) + 1.0f;
  add_player(simulation, &temp);
}



int main(int argc, char **argv){

  engine_t engine = {0};
  pthread_barrier_init(&engine.barrier, NULL, 3);


  init_sdl_engine(&engine.gui);
  generate_meshes(&engine.mesh_table);
  generate_shaders(&engine.shader_table);

  generate_terrain(&engine.simulation, &engine.mesh_table);
  create_entities(&engine.simulation, &engine.mesh_table, &engine.shader_table);
  create_player(&engine.simulation);

  init_renderer(&engine);
  //init_console(&engine.simulation);
  init_controller(&engine.controller);




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
  while (engine.modes.running) {

    engine.controller.dt = cycle_time(&event_cycle);
    event_handler(&engine.controller, &engine.simulation, &engine.modes);
    step_simulation(&engine.simulation);
    step_physics(&engine.controller, &engine.simulation);

  }

  cleanup(&engine);
  return 0;
}




