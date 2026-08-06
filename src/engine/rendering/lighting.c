//
// Created by berserker on 8/1/26.
//

#include "lighting.h"

#include <SDL_timer.h>
#include "../rendering/render.h"


void update_sun_direction(sun_cycle_t *sun_c, sun_t *sun){
  if (!sun_c->visible) {
    sun->direction = vec3_fill(0);
    return;
  }

  float radians = sun_c->angle * ((float)M_PI / 180.0f);

  sun->direction = (vec3){
    cosf(radians),
    -sinf(radians),
     0.0f
};
}

void update_lighting(lighting_t *lighting, sun_t *sun){

  u32 now = SDL_GetTicks();

  float elapsed = (float)(now - sun->cycle.start_time) / 270.0f;

  if(sun->cycle.visible){

    sun->cycle.angle = (elapsed / 300.0f) * 270.0f;
    if(sun->cycle.angle<90 && sun->cycle.angle>0){
      lighting->ambient = sun->cycle.angle / 100.0f + 0.1f;
    }else if(sun->cycle.angle>90 && sun->cycle.angle<180){
      lighting->ambient = (180.0f - sun->cycle.angle) / 100.0f + 0.1f;
    }
    if(lighting->ambient > 0.3) lighting->ambient = 0.3f;


    if(elapsed >= 300.0f){
      sun->cycle.start_time = now;
      sun->cycle.visible = 0;
      sun->cycle.angle = 270.0f;
    }

  }else{
    lighting->ambient = 0.1f;
    if(elapsed >= 10.0f){
      sun->cycle.start_time = now;
      sun->cycle.visible = 1;
      sun->cycle.angle = -90.0f;
    }

  }


  update_sun_direction(&sun->cycle, sun);

  lighting->direction = sun->direction;
  lighting->color = sun->color;
  lighting->intensity = sun->intensity;
}

sun_t new_sun(void) {
  sun_t sun = {
    .direction = {1.0f, 0.0f, 0.0f},
    .color = {1.0f, 0.95f, 0.8f},
    .intensity = 1.0f,
    .cycle = (sun_cycle_t){
      .start_time = SDL_GetTicks(),
      .angle = 0.0f,
      .visible = 1
    }
  };
  return sun;
}
