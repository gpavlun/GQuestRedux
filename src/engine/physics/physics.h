//
// Created by berserker on 7/24/26.
//

#ifndef GQUESTREDUX_PHYSICS_H
#define GQUESTREDUX_PHYSICS_H

#include "../main/typing.h"

#include "../math/vector.h"

def logical_chunk_t logical_chunk_t;
def controller_state_t controller_state_t;
def simulation_t simulation_t;

float terrain_height(vec3 pos, logical_chunk_t *terrain);
void step_physics(controller_state_t *controller, simulation_t *simulation);

#endif //GQUESTREDUX_PHYSICS_H
