//
// Created by berserker on 8/1/26.
//

#ifndef GQUEST_PLAYER_CONTROLLER_H
#define GQUEST_PLAYER_CONTROLLER_H

#include "game.h"


#include "../physics/physics.h"

#define $jump_interval 200
#define $walkspeed 6
#define $strafespeed ($walkspeed/1.3f)

void event_handler(controller_state_t *controller, simulation_t *simulation, boolean_t *modes);
void player_controller(player_t *player, inputs_t key);
void init_controller(controller_state_t *controller);

#endif //GQUEST_PLAYER_CONTROLLER_H
