//
// Created by berserker on 7/24/26.
//

#ifndef GQUESTREDUX_PHYSICS_H
#define GQUESTREDUX_PHYSICS_H

#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <gio.h>

#include "../main/game.h"


float terrain_height(vec3 pos, logical_chunk_t *terrain);
void step_physics(controller_state_t *controller, simulation_t *simulation);

#endif //GQUESTREDUX_PHYSICS_H
