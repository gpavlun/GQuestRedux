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

#include <gio.h>

#include "game.h"
#include "typing.h"
#include "vector.h"
#include "matrix.h"


def {

    float max_force;
    float mass;
    float normal_vel;
    float normal_acc;

    float theta;
    float phi;
    vec3 pos;
    vec3 vel;
    vec3 acc;
    vec3 desired_velocity;
    vec3 applied_force;
    vec3 net_force;

}actor_t;
extern actor_t *player_glob;
  
def {
    float psi, theta, phi;

    vec3 pos;
    mat4 view;
    mat4 projection;  
}camera_t;
extern camera_t *camera_glob;

extern u8 flying;



def {
    u8 w : 1;
    u8 a : 1;
    u8 s : 1;
    u8 d : 1;
    u8 space : 1;
    u8 shift : 1;
}inputs_t;


float terrain_height(vec3 pos);

#endif //GQUESTREDUX_PHYSICS_H
