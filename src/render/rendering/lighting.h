//
// Created by berserker on 8/1/26.
//

#ifndef GQUEST_LIGHTING_H
#define GQUEST_LIGHTING_H

#include "../math/vector.h"

def {
  u32 start_time;
  float angle;
  int visible;
}sun_cycle_t;

def {
  vec3 direction;
  vec3 color;
  float intensity;
  sun_cycle_t cycle;
}sun_t;

def {
  vec3 direction;
  vec3 color;
  float intensity;
  float ambient;
}lighting_t;

sun_t new_sun(void);
void update_sun_direction(sun_cycle_t *sun_c, sun_t *sun);
void update_lighting(lighting_t *lighting, sun_t *sun);

#endif //GQUEST_LIGHTING_H
