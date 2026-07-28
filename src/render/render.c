// clang-format off

#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <glad/glad.h>
#include <logging.h>

#include "editor.h"
#include "render.h"
#include "game.h"
#include "physics.h"



int max(int a, int b, int c) {
    if(a>=b && a>=c) return a;
    if(b>=a && b>=c) return b;
    return c;
}

int min(int a, int b, int c){
    if(a<=b && a<=c) return a;
    if(b<=a && b<=c) return b;
    return c;
}

char *read_file(char *path){
  size_t nread;

  FILE *file = fopen(path, "rb");
  logging.assert(file, "file not found!");

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  if(size< 0){
      fclose(file);
      return NULL;
  }

  char *source = malloc(size + 1);
  nread = fread(source, 1, size, file);
  source[nread] = 0;

  fclose(file);

  return source;
}


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
  float radians = fov * (M_PI / 180.0f);
  float f = 1.0f / tan(radians / 2.0f);  
  
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

mat4 mat4_translate_camera(vec3_t pos){
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
mat4 mat4_translate_pos(vec3_t pos){
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

void start_render(void) {
  gui_engine_t gui = gui_engine_init();
  init_events(&gui);

  window_t *window = &gui.window;
  

  // create the gpu connection
  SDL_GLContext gl_context = SDL_GL_CreateContext(gui.sdl2.window); 

  // bind the gpu functions to this script               
  logging.assert(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress),
                 "GLAD failed"); 
  
  
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.486f, 0.686f, 0.761f, 1.0f);

  GLint gl_success_code;
  hexcode_u color;



  player.pos.x = 0;
  player.pos.y = 2;
  player.pos.z = 0;



  /*** create vertex shader ***/

  // read in shader data and create a context for it
  char *source = read_file("./src/render/basic.vert");
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  logging.assert(vertex_shader, "GL vertex failed!");

  // load shader source
  glShaderSource(
      vertex_shader,
      1,
      (const GLchar * const*)&source,
      NULL
  );

  // compile shader source
  glCompileShader(vertex_shader);
  
  // check for compile failure
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetShaderInfoLog(vertex_shader, 512, NULL, log);
    logging.error(0, log);
  }

  free(source);



  /*** create fragment shader ***/

  // read in shader data and create a context for it
  source = read_file("./src/render/basic.frag");
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  logging.assert(fragment_shader, "GL fragment failed!");

  // load shader source
  glShaderSource(
      fragment_shader,
      1,
      (const GLchar * const*)&source,
      NULL
  );

  // compile shader source
  glCompileShader(fragment_shader);

  // check for compile failure
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetShaderInfoLog(fragment_shader, 512, NULL, log);
    logging.error(0, log);
  }

  free(source);



  /*** link the vertex and fragment shaders ***/

  // create a program object
  GLuint program = glCreateProgram();
  logging.assert(program, "GL program failed!");

  // attach the shaders to the object
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  // finish the linking
  glLinkProgram(program);

  // check for linking failure
  glGetProgramiv(program, GL_LINK_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetProgramInfoLog(program, 512, NULL, log);
    logging.error(0, log);
  }

  // use the linked program
  glUseProgram(program);



  /*** Load player position in variable ***/


  GLint projection_loc;

  projection_loc = glGetUniformLocation(
      program,
      "projection"
  );  

  logging.assert(projection_loc != -1,
                "projection not found");

  GLint view_loc;

  view_loc = glGetUniformLocation(
      program,
      "view"
  );  

  logging.assert(projection_loc != -1,
                "projection not found");

  GLint model_loc;

  model_loc = glGetUniformLocation(
      program,
      "model"
  );  

  logging.assert(model_loc != -1,
                "model not found");

 


  vert_t vertices[3] = {
    { 1 , 1 , 0 },
    { 0 ,-1 , 0 },
    {-1 , 1 , 0 }
  };



  /*** create vertex array ***/

  // create empty vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // create vertex buffer object
  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

  // define and set the vertex buffer
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(vertices),
      vertices,
      GL_STATIC_DRAW
  );

  // define how vertex buffer should be interpreted
  glVertexAttribPointer(
      0,                  // shader location defined in shader
      3,                  // three values per index
      GL_FLOAT,           // each index value is a float
      GL_FALSE,           // normalization
      sizeof(vert_t),     // size of one index
      NULL                // struct offset
  );

  // enable this array
  glEnableVertexAttribArray(0);



  u32 indices[3];
  indices[0] = 0;
  indices[1] = 1;
  indices[2] = 2;



  /*** triangle index map ***/
  
  // create index object
  GLuint index_buffer;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);

  // define and set index buffer
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(indices),
      indices,
      GL_STATIC_DRAW
  );


  mat4 projection = mat4_perspective(
      70.0f,        // field of view
      (float)gui.window.dim.w/(float)gui.window.dim.h, // aspect ratio
      0.1f,         // near plane
      1000.0f       // far plane
  );

  mat4 view;
  vec3_t objpos = {0, 0,-10};
  mat4 model;

  model = mat4_translate_pos(objpos);
  glUniformMatrix4fv(
    model_loc,
    1,
    GL_FALSE,
    model.i
  );

  glUniformMatrix4fv(
    projection_loc,
    1,
    GL_FALSE,
    projection.i
  );    

  GLenum gl_err;
  while(modes.RUNNING){

    // clear the buffer colors and depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the progam and vao for this object
    glUseProgram(program);

    
    view = mat4_identity();
    view = mat4_mul(mat4_translate_camera(player.pos), view);
    view = mat4_mul(mat4_rotate_y(player.theta), view);
    view = mat4_mul(mat4_rotate_x(player.phi), view);
    glUniformMatrix4fv(
        view_loc,
        1,
        GL_FALSE,
        view.i
    );


    glBindVertexArray(vao);

    // draw the object
    glDrawElements(
        GL_TRIANGLES,
        3,
        GL_UNSIGNED_INT,
        0
    );

    // check for errors
    gl_err = glGetError();
    while(gl_err != GL_NO_ERROR){
        logging.warn("render failure");
        gl_err = glGetError();
    }

    // load the window
    SDL_GL_SwapWindow(gui.sdl2.window);

    //usleep(event_rate);
  }


  SDL_DestroyWindow(gui.sdl2.window);
  SDL_Quit();
}