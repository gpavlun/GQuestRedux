//
// Created by berserker on 7/24/26.
//

#ifndef GQUESTREDUX_PHYSICS_H
#define GQUESTREDUX_PHYSICS_H

#include "editor.h"
#include "game.h"

def {
    float x, y, z;
}vec3_t;

def {

    float max_force;
    float mass;
    float normal_vel;
    float normal_acc;
    float theta;
    float phi;

    vec3_t pos;
    vec3_t vel;
    vec3_t acc;
    vec3_t desired_velocity;
    vec3_t applied_force;
    vec3_t net_force;

}actor_t;
extern actor_t player_glob;
extern u8 flying;

def {
    float x, y, z;
}vert_t;

def {
    u32 A, B;
}edge_t;

def {
    u16 a, b, c;
    hexcode_u color;
}triangle_t;

def {
    u8 wire_frame: 1;
    u8 triangles: 1;
    u8 vertices: 1;
}mesh_opts;

def {
    u8 w : 1;
    u8 a : 1;
    u8 s : 1;
    u8 d : 1;
    u8 space : 1;
    u8 shift : 1;
}inputs_t;

#endif //GQUESTREDUX_PHYSICS_H
