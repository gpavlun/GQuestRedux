#include "simulation.h"

#include "../main/game.h"

void rotate_object(entity_t *object) {

  static u32 old;
  u32 now = SDL_GetTicks();
  u32 elapsed = now - old;
  if (elapsed > 10) {
    old = now;
    object->transform.rot.y += 0.1f;
    if (object->transform.rot.y > 6.28) object->transform.rot.y = 0;
    object->render_data.remodel = true;
  }
}

void step_simulation(simulation_t *simulation) {
  rotate_object(&simulation->entity_table[0]);
}