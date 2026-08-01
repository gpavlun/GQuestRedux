//
// Created by berserker on 7/29/26.
//

#ifndef GQUEST_VECTOR_H
#define GQUEST_VECTOR_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "../main/typing.h"

def {
  float x, y, z;
}vec3;

def {
  float x, y;
}vec2;

static inline vec3 vec3_add(vec3 vec1, vec3 vec2) {
  return (vec3){vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z};
}

static inline vec3 vec3_sub(vec3 vec1, vec3 vec2) {
  return (vec3){vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z};
}

static inline vec3 vec3_scale(vec3 vec, float scalar) {
  return (vec3){vec.x * scalar, vec.y * scalar, vec.z * scalar};
}

static inline float vec3_dot(vec3 vec1, vec3 vec2) {
  return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
}

static inline vec3 vec3_cross(vec3 vec1, vec3 vec2) {
  return (vec3){
    ( vec1.y * vec2.z - vec1.z * vec2.y),
    ( vec1.x * vec2.z - vec1.z * vec2.x),
    ( vec1.x * vec2.y - vec1.y * vec2.x)
  };
}

static inline float vec3_length(vec3 vec) {
  return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

static inline vec3 vec3_neg(vec3 vec) {
  return (vec3){-vec.x, -vec.y, -vec.z};
}

static inline vec3 vec3_fill(float value) {
  return (vec3){value, value, value};
}

static inline vec3 vec3_new(float a, float b, float c) {
  return (vec3){a, b, c};
}

static inline vec3 vec3_div(vec3 vec, float scalar){
  return vec3_scale(vec, 1.0f / scalar);
}
static inline float vec3_distance(vec3 a, vec3 b){
  return vec3_length(vec3_sub(a,b));
}

static inline vec3 vec3_normalize(vec3 vec) {
  float len = vec3_length(vec);
  if (len == 0.0f) return vec;
  return vec3_scale(vec, 1.0f / len);
}


#endif //GQUEST_VECTOR_H
