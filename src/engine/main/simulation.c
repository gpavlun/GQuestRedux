#include "simulation.h"

#include "../main/game.h"

void rotate_object(entity_t *object) {

  static u32 old;
  u32 now = SDL_GetTicks();
  u32 elapsed = now - old;
  if (elapsed > 20) {
    old = now;
    object->transform.rot = vec3_add(object->transform.rot, (vec3){0.1f, 0.1f, 0.1f});
    if (object->transform.rot.y > 6.28) object->transform.rot = (vec3){0, 0, 0};
    object->render_data.remodel = true;
  }
}

void step_simulation(simulation_t *simulation) {
  rotate_object(&simulation->entity_table[1]);
  simulation->player_table[0].entity.render_data.remodel = true;
}