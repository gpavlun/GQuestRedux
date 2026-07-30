//
// Created by berserker on 7/24/26.
//

#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <gio.h>
#include "physics.h"





#define DRAG_CONSTANT 200
#define planetary_mass 10000000000000000000000000
#define planitary_diam 12756000
#define gravitational_const 0.000000000067408
#define cheat_grav 9.8f
#define friction_coeff 1.6f

#define sensitivity 0.5f




actor_t player_glob;


float phys_clamp(float value, float min, float max){
  if(value>max) return max;
  if(value<min) return min;
  return value;
}
void flying_movement_physics(actor_t *actor, float dt){
    float c = cosf(actor->theta);
    float s = sinf(actor->theta);

    vec3_t desired_world;
    desired_world.x = player_glob.desired_velocity.x * c +
                      player_glob.desired_velocity.z * s;
    desired_world.y = player_glob.desired_velocity.y;
    desired_world.z = -player_glob.desired_velocity.x * s +
                       player_glob.desired_velocity.z * c;



    player_glob.applied_force.x = player_glob.max_force *
              (desired_world.x - player_glob.vel.x);
    player_glob.applied_force.y = player_glob.max_force *
              (desired_world.y - player_glob.vel.y);
    player_glob.applied_force.z = player_glob.max_force *
              (desired_world.z - player_glob.vel.z);

    float force_len = sqrtf(
        player_glob.applied_force.x * player_glob.applied_force.x +
        player_glob.applied_force.y * player_glob.applied_force.y +
        player_glob.applied_force.z * player_glob.applied_force.z
    );

    if(force_len > player_glob.max_force){
        float scale = player_glob.max_force / force_len;
        player_glob.applied_force.x *= scale;
        player_glob.applied_force.y *= scale;
        player_glob.applied_force.z *= scale;
    }

    float normal_force = 0.0f;
    float friction_force;

    normal_force = player_glob.mass * cheat_grav;
    friction_force = 3 * friction_coeff * normal_force;

    vec3_t net_force;
    net_force.x = player_glob.applied_force.x;
    net_force.y = player_glob.applied_force.y;
    net_force.z = player_glob.applied_force.z;
    float d3_force = sqrtf(
        net_force.x * net_force.x +
        net_force.y * net_force.y +
        net_force.z * net_force.z
    );
    if(d3_force > friction_force){
      float scale = friction_force / d3_force;
      net_force.x *= scale;
      net_force.y *= scale;
      net_force.z *= scale;
    }

    player_glob.acc.x = net_force.x / player_glob.mass;
    player_glob.acc.y = net_force.y / player_glob.mass;
    player_glob.acc.z = net_force.z / player_glob.mass;

    player_glob.vel.x += player_glob.acc.x * dt;
    player_glob.vel.y += player_glob.acc.y * dt;
    player_glob.vel.z += player_glob.acc.z * dt;

    player_glob.pos.x += player_glob.vel.x * dt;
    player_glob.pos.y += player_glob.vel.y * dt;
    player_glob.pos.z += player_glob.vel.z * dt;

    player_glob.net_force = net_force;
}
void movement_physics(actor_t *actor, float dt){
    float c = cosf(actor->theta);
    float s = sinf(actor->theta);

    vec3_t desired_world;
    desired_world.x = player_glob.desired_velocity.x * c +
                      player_glob.desired_velocity.z * s;
    desired_world.z = -player_glob.desired_velocity.x * s +
                       player_glob.desired_velocity.z * c;
    desired_world.y = player_glob.desired_velocity.y;


    player_glob.applied_force.x = player_glob.max_force *
              (desired_world.x - player_glob.vel.x);
    player_glob.applied_force.z = player_glob.max_force *
              (desired_world.z - player_glob.vel.z);

    float force_len = sqrtf(
        player_glob.applied_force.x * player_glob.applied_force.x +
        player_glob.applied_force.z * player_glob.applied_force.z
    );

    if(force_len > player_glob.max_force){
        float scale = player_glob.max_force / force_len;
        player_glob.applied_force.x *= scale;
        player_glob.applied_force.z *= scale;
    }

    float gravity_force = player_glob.mass * -cheat_grav;
    float normal_force = 0.0f;
    float friction_force;


    player_glob.applied_force.y = 0;
    if(player_glob.pos.y <= 2.0f){

        if(player_glob.pos.y < 2.0f){
          player_glob.pos.y = 2.0f;
          if(player_glob.vel.y < 0) player_glob.vel.y = 0;
          if(player_glob.acc.y > 0) player_glob.acc.y = 0;
        }
        normal_force = player_glob.mass * cheat_grav;
    }
    friction_force = friction_coeff * normal_force;



    vec3_t net_force;

    net_force.y = gravity_force + normal_force;
    net_force.x = player_glob.applied_force.x;
    net_force.z = player_glob.applied_force.z;

    float horizontal_force = sqrtf(
        net_force.x * net_force.x +
        net_force.z * net_force.z
    );

    if(horizontal_force > friction_force){
        float scale = friction_force / horizontal_force;
        net_force.x *= scale;
        net_force.z *= scale;
    }

    player_glob.acc.x = net_force.x / player_glob.mass;
    player_glob.acc.y = net_force.y / player_glob.mass;
    player_glob.acc.z = net_force.z / player_glob.mass;

    player_glob.vel.x += player_glob.acc.x * dt;
    player_glob.vel.y += player_glob.acc.y * dt;
    player_glob.vel.z += player_glob.acc.z * dt;

    player_glob.pos.x += player_glob.vel.x * dt;
    player_glob.pos.y += player_glob.vel.y * dt;
    player_glob.pos.z += player_glob.vel.z * dt;

    player_glob.net_force = net_force;
}

#define jump_interval 200
#define walkspeed 6
#define strafespeed (walkspeed/1.3f)
u8 flying;
void multipress(inputs_t key){
  player_glob.desired_velocity.x = player_glob.desired_velocity.y = player_glob.desired_velocity.z = 0;
  if(!flying) player_glob.applied_force.y = 0;
  float input_x = 0, input_y = 0, input_z = 0;
  static u64 last_jump;

  if(key.w){
    input_z += 1;
  }

  if(key.s){
    input_z -= 1;
  }

  if(key.a){
    input_x -= 1;
  }

  if(key.d){
    input_x += 1;
  }

  if(key.space){
    input_y += 1;
  }
  if(key.shift){
    input_y -= 1;
  }

  if(input_y){
    u64 now;
    now = SDL_GetTicks64();
    if(input_y>0 && now - last_jump < jump_interval && now - last_jump > jump_interval/3){
      flying = !flying;
      if(flying){
        player_glob.vel.x = 0;
        player_glob.vel.y = 0;
        player_glob.vel.z = 0;
      }
    }
    if(!flying && player_glob.pos.y<=2){
      player_glob.vel.y = walkspeed;
    }
    last_jump = now;
  }

  float len;
  if(flying){
    len = sqrtf(input_x * input_x + input_y * input_y + input_z * input_z);
    if(len>0){
      input_x /= len;
      input_y /= len;
      input_z /= len;
    }
  }else{
    len = sqrtf(input_x * input_x + input_z * input_z);
    if(len>0){
      input_x /= len;
      input_z /= len;
    }
  }



  if(flying){
    player_glob.desired_velocity.x = input_x * walkspeed * 2;
    player_glob.desired_velocity.y = input_y * walkspeed * 2;
    player_glob.desired_velocity.z = input_z * walkspeed * 2;
  }else{
    player_glob.desired_velocity.x = input_x * strafespeed;
    player_glob.desired_velocity.z = input_z *  (input_z>0.0f ? walkspeed : strafespeed);
  }
}

void *editor_event_handler(void *args){
  SDL_Event event;
  const uint8_t *keyscan = SDL_GetKeyboardState(NULL);
  inputs_t inputs = {0};
  u64 last_movement = SDL_GetPerformanceCounter();

  u64 current_movement;
  float dt;

  player_glob.mass = 80;
  player_glob.max_force = 2745;
  flying = 0;
  u8 mousemode = 0;

  while(modes.RUNNING){
    current_movement = SDL_GetPerformanceCounter();
    if(!flying) inputs.space = 0;
    dt = (float)(current_movement - last_movement) /
         (float)SDL_GetPerformanceFrequency();

    while(SDL_PollEvent(&event)){
      switch(event.type){
        case(SDL_QUIT):{

          modes.RUNNING = 0;

        $}case(SDL_KEYUP):{
          switch(event.key.keysym.sym){
            case(SDLK_ESCAPE):{
              modes.RUNNING = 0;
            $}default:{$}
          }
        $}case(SDL_KEYDOWN):{
          switch(event.key.keysym.sym){
            case(SDLK_SPACE):{
              if(!flying && !event.key.repeat){
                inputs.space = 1;
              }
            $}default:{$}
          }

        $}case(SDL_MOUSEMOTION):{
          if(mousemode){
            player_glob.theta += (float)event.motion.xrel * 0.01f;
            player_glob.phi   -= (float)event.motion.yrel * 0.01f;

            if(player_glob.phi>1.5) player_glob.phi = 1.5f;
            else if(player_glob.phi<-1.5) player_glob.phi = -1.5f;
          }
        $}case(SDL_MOUSEBUTTONUP):{
          if(mousemode) SDL_SetRelativeMouseMode(SDL_FALSE);
          else SDL_SetRelativeMouseMode(SDL_TRUE);

          mousemode = !mousemode;
        $}default:{$}
      }

      inputs.w = keyscan[SDL_SCANCODE_W];
      inputs.a = keyscan[SDL_SCANCODE_A];
      inputs.s = keyscan[SDL_SCANCODE_S];
      inputs.d = keyscan[SDL_SCANCODE_D];

      if(flying){
        inputs.shift = keyscan[SDL_SCANCODE_LSHIFT];
        inputs.space = keyscan[SDL_SCANCODE_SPACE];
      }

    }
    multipress(inputs);
    if(flying){
      flying_movement_physics(&player_glob, dt);
    }else{
      movement_physics(&player_glob, dt);
    }


    last_movement = current_movement;
    usleep(event_rate);
  }

  pthread_exit(0);
}



