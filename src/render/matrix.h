#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "typing.h"
#include "vector.h"

def {
  float i[16];
}mat4;

def {
  int x, y;
}point_t;

def {
  int x, y;
  u8 visible;
}vpoint_t;




static inline float mat4_get(mat4 *m, int row, int col);
static inline void mat4_set(mat4 *m, int row, int col, float value);
mat4 mat4_mul(mat4 a, mat4 b);
mat4 mat4_identity(void);
mat4 mat4_perspective(float fov, float aspect_ratio, float near, float far);
mat4 mat4_translate_camera(vec3 pos);
mat4 mat4_translate_pos(vec3 pos);
mat4 mat4_rotate_y(float theta);
mat4 mat4_rotate_x(float phi);
mat4 mat4_rotate_z(float psi);
mat4 mat4_scale(vec3 scale);

#endif //MATRIX_H