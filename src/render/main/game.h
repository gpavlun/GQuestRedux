#ifndef GAME_H
#define GAME_H

#define event_rate 10000 //100x a sec

#include <stdbool.h>

#include "../main/typing.h"
#include "../math/vector.h"
#include "../math/matrix.h"
#include "../rendering/render.h"

#include "../sdl_windowing/sdl_ops.h"

typedef struct boolean_struct{
  u8 running: 1;
  u8 dynamic: 1;
  u8 wireframe:1;

}boolean_t;

def {
  u8 w : 1;
  u8 a : 1;
  u8 s : 1;
  u8 d : 1;
  u8 space : 1;
  u8 shift : 1;
}inputs_t;

def {
  vec3 pos;
  vec3 rot;
  vec3 scale;
}transform_t;


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

def {
  vec3 rot;
  vec3 pos;
  mat4 view;
  mat4 projection;

  vec3 offset;
}camera_t;




def {
  inputs_t inputs;
  const uint8_t *keyscan;
  u8 mousemode;
  float dt;
}controller_state_t;

enum mesh_table_index_e {
  $demo_tri_mesh = 0,
  $barad_dur_mesh = 1,
  $barad_eye_mesh = 2,
  $player_mesh = 3,
  $tower_mesh = 4,
  $tower_roof_mesh = 5,
  $castle_mesh = 6,
  $castle_roof_mesh = 7,
  $terrain_mesh = 8
};




def {

  float max_force;
  float mass;
  float normal_vel;
  float normal_acc;

  vec3 vel;
  vec3 acc;
  vec3 desired_velocity;
  vec3 applied_force;
  vec3 net_force;

}physics_actor_t;

def {
  bool hidden;

  size_t mesh_idx;
  size_t shader_idx;

  bool remodel;
  mat4 model;
}render_data_t;

def {
  transform_t transform;
  physics_actor_t physics_actor;
  render_data_t render_data;
}entity_t;

def {
  entity_t entity;
  camera_t camera;

  bool perspective;
  bool flying;
}player_t;

def {
  logical_chunk_t *terrain_table;
  size_t nterrains;
  entity_t *entity_table;
  size_t nentities;
  player_t *player_table;
  size_t nplayers;

}simulation_t;

def {
  shader_t *shader;
  size_t nshaders;
}shader_table_t;

def {
  mesh_t *mesh;
  size_t nmeshes;
}mesh_table_t;
// new model, simulation has entities, the enities have indexes
// for their mesh in the mesh table. Then access is relegated to
// the enitity table for all subsystems, the idea being that the
// subsystems do not need to have straight access to other
// systems data, with the entity table being a bridge of sorts.
def {
  boolean_t modes;
  gui_data_t gui;

  controller_state_t controller;
  simulation_t simulation;
  mesh_table_t mesh_table;
  shader_table_t shader_table;

  pthread_barrier_t barrier;

  pthread_t render_thread;
  pthread_t console_thread;
}engine_t;

#endif
