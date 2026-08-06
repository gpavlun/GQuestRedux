#include "matrix.h"

/*

Matrices are COLUMN major, so all elements are m[col][row].
The gpu interface was programmed this way, I don't know why.

*/


static inline float mat4_get(mat4 *m, int row, int col){
    return m->i[col * 4 + row];
}
static inline void mat4_set(mat4 *m, int row, int col, float value){
    m->i[col * 4 + row] = value;
}
mat4 mat4_mul(mat4 a, mat4 b){
    mat4 result = {0};

    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            float value = 0;

            for(int k = 0; k < 4; k++){
                value +=
                    mat4_get(&a, row, k) *
                    mat4_get(&b, k, col);
            }

            mat4_set(&result, row, col, value);
        }
    }

    return result;
}
mat4 mat4_identity(void){
    mat4 result = {
        .i = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        }
    };

    return result;
}




mat4 mat4_perspective(float fov, float aspect_ratio, float near, float far){
  float radians = fov * ((float)M_PI / 180.0f);
  float f = 1.0f / tanf(radians / 2.0f);
  
  float a = aspect_ratio;

  float A = (far + near) / (near - far);
  float B = (2 * far * near) / (near - far); 
  
  mat4 perspective = {
    .i = {
      f/a, 0 , 0 , 0 ,
       0 , f , 0 , 0 ,
       0 , 0 , A ,-1 ,
       0 , 0 , B , 0
    }
  };

  return perspective;
}

mat4 mat4_translate_camera(vec3 pos){
    mat4 view = {
        .i = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -pos.x, -pos.y, -pos.z, 1
        }
    };

    return view;
}
mat4 mat4_translate_pos(vec3 pos){
    mat4 view = {
        .i = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            pos.x, pos.y, pos.z, 1
        }
    };

    return view;
}

mat4 mat4_rotate_y(float theta){
    float c = cosf(theta);
    float s = sinf(theta);

    mat4 result = {
        .i = {
             c , 0 , s , 0 ,
             0 , 1 , 0 , 0 ,
            -s , 0 , c , 0 ,
             0 , 0 , 0 , 1
        }
    };

    return result;
}

mat4 mat4_rotate_x(float phi){
    float c = cosf(phi);
    float s = sinf(phi);

    mat4 result = {
        .i = {
            1, 0, 0, 0,
            0, c, s, 0,
            0,-s, c, 0,
            0, 0, 0, 1
        }
    };

    return result;
}

mat4 mat4_rotate_z(float psi){
    mat4 m = mat4_identity();

    float c = cosf(psi);
    float s = sinf(psi);

    m.i[0] =  c;
    m.i[1] =  s;

    m.i[4] = -s;
    m.i[5] =  c;

    return m;
}

mat4 mat4_scale(vec3 scale){
    mat4 m = mat4_identity();

    m.i[0]  = scale.x;
    m.i[5]  = scale.y;
    m.i[10] = scale.z;

    return m;
}