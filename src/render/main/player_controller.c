//
// Created by berserker on 8/1/26.
//

#include "player_controller.h"
#include "../physics/physics.h"

void player_controller(player_t *player, inputs_t key, simulation_t *simulation){
  player->entity.physics_actor.desired_velocity.x = 0;
  player->entity.physics_actor.desired_velocity.y = 0;
  player->entity.physics_actor.desired_velocity.z = 0;
  float input_x = 0, input_y = 0, input_z = 0;
  static u64 last_jump;

  if(key.w) input_z -= 1;
  if(key.s) input_z += 1;
  if(key.a) input_x -= 1;
  if(key.d) input_x += 1;
  if(key.space) input_y += 1;
  if(key.shift) input_y -= 1;

  if(input_y){
    u64 now;
    now = SDL_GetTicks64();

    if(input_y>0 && now - last_jump < $jump_interval && now - last_jump > $jump_interval/3){
       player->flying = ! player->flying;
      if( player->flying) player->entity.physics_actor.vel = (vec3){0,0,0};
    }

    float height = terrain_height(player->entity.transform.pos, &simulation->terrain_table[0]);
    if(!player->flying && player->entity.transform.pos.y<=(height + 0.1f)){
      player->entity.physics_actor.vel.y = $walkspeed*.8f;
    }

    last_jump = now;
  }

  float len;
  if(player->flying){

    len = sqrtf(input_x * input_x + input_y * input_y + input_z * input_z);
    if(len>0){
      input_x /= len;
      input_y /= len;
      input_z /= len;
    }
    player->entity.physics_actor.desired_velocity.x = input_x * $walkspeed * 2;
    player->entity.physics_actor.desired_velocity.y = input_y * $walkspeed * 2;
    player->entity.physics_actor.desired_velocity.z = input_z * $walkspeed * 2;

  }else{

    len = sqrtf(input_x * input_x + input_z * input_z);
    if(len>0){
      input_x /= len;
      input_z /= len;
    }
    player->entity.physics_actor.desired_velocity.x = input_x * $strafespeed;
    player->entity.physics_actor.desired_velocity.z = input_z *  (input_z<0.0f ? $walkspeed : $strafespeed);

  }
}


void init_controller(controller_state_t *controller) {
  controller->mousemode = 0;
  controller->keyscan = SDL_GetKeyboardState(NULL);
  controller->inputs = (inputs_t){0};
}


void event_handler(
  controller_state_t *controller,
  simulation_t *simulation,
  boolean_t *modes
  ){

  if(!simulation->player_table[0].flying) controller->inputs.space = 0;

  SDL_Event event;
  while(SDL_PollEvent(&event)){
    switch(event.type){

      /***** when the window is closed *****/
      case SDL_QUIT:

        modes->running = false;

      /***** when key is released *****/
      $ case SDL_KEYUP:

        switch(event.key.keysym.sym){
          case SDLK_ESCAPE:
            modes->running = false;
          $ case SDLK_p:
            modes->wireframe = !modes->wireframe;
          $ case SDLK_F4:
              simulation->player_table[0].perspective = !simulation->player_table[0].perspective;
              if (simulation->player_table[0].perspective) {
                simulation->player_table[0].camera.offset = (vec3){0, 3, 2};
              }else {
                simulation->player_table[0].camera.offset = (vec3){0, 2, 0};
              }

          $ endcase
        }

      /***** when key is pressed *****/
      $ case SDL_KEYDOWN:

        switch(event.key.keysym.sym){
          case SDLK_SPACE:
            if(!simulation->player_table[0].flying && !event.key.repeat){
              controller->inputs.space = 1;
            }
          $ endcase
        }

      /***** when mouse is moved *****/
      $ case SDL_MOUSEMOTION:

        if(controller->mousemode){
          simulation->player_table[0].camera.rot.y -= (float)event.motion.xrel * 0.01f;
          simulation->player_table[0].camera.rot.x   += (float)event.motion.yrel * 0.01f;

          if(simulation->player_table[0].camera.rot.x>1.55)
            simulation->player_table[0].camera.rot.x = 1.55f;
          else
          if(simulation->player_table[0].camera.rot.x<-1.55)
            simulation->player_table[0].camera.rot.x = -1.55f;
        }

      /***** when mouse scrolls *****/
      $ case SDL_MOUSEWHEEL:

        if(controller->mousemode){

          simulation->player_table[0].camera.rot.z -= (float)event.wheel.y * 0.1f;

          if(simulation->player_table[0].camera.rot.z >  1.55)
            simulation->player_table[0].camera.rot.z  = 1.55f;
          else
          if(simulation->player_table[0].camera.rot.z < -1.55)
            simulation->player_table[0].camera.rot.z = -1.55f;
        }

      /***** when mouse button is released *****/
      $ case SDL_MOUSEBUTTONUP:

        if(controller->mousemode) SDL_SetRelativeMouseMode(SDL_FALSE);
        else SDL_SetRelativeMouseMode(SDL_TRUE);

        controller->mousemode = !controller->mousemode;

      $ endcase
    }

    controller->inputs.w = controller->keyscan[SDL_SCANCODE_W];
    controller->inputs.a = controller->keyscan[SDL_SCANCODE_A];
    controller->inputs.s = controller->keyscan[SDL_SCANCODE_S];
    controller->inputs.d = controller->keyscan[SDL_SCANCODE_D];

    if(simulation->player_table[0].flying){
      controller->inputs.shift = controller->keyscan[SDL_SCANCODE_LSHIFT];
      controller->inputs.space = controller->keyscan[SDL_SCANCODE_SPACE];
    }

  }

  player_controller(&simulation->player_table[0], controller->inputs, simulation);
  simulation->player_table[0].entity.transform.rot = simulation->player_table[0].camera.rot;
}