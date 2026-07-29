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
#include "matrix.h"
#include "filehelper.h"



shader_t init_shader(char *vert_path, char *frag_path){

  GLint gl_success_code;
  shader_t shader = {0};



  /*** create vertex shader ***/

  // read in shader data and create a context for it
  char *source = read_file(vert_path);
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
  source = read_file(frag_path);
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
  shader.program = glCreateProgram();
  logging.assert(shader.program, "GL program failed!");

  // attach the shaders to the object
  glAttachShader(shader.program, vertex_shader);
  glAttachShader(shader.program, fragment_shader);

  // finish the linking
  glLinkProgram(shader.program);

  // check for linking failure
  glGetProgramiv(shader.program, GL_LINK_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetProgramInfoLog(shader.program, 512, NULL, log);
    logging.error(0, log);
  }



  /*** variable declarations for program ***/

  shader.projection = glGetUniformLocation(shader.program, "projection");  
  logging.assert(shader.projection != -1, "projection not found");

  shader.view = glGetUniformLocation(shader.program, "view");  
  logging.assert(shader.view != -1, "projection not found");

  shader.model = glGetUniformLocation(shader.program, "model");  
  logging.assert(shader.model != -1, "model not found");

  return shader;

}

mesh_t init_mesh(size_t nverts, vertex_t *verts, size_t ntris, tri_t *tris){

  mesh_t mesh = {0};

  mesh.nverts = nverts;
  mesh.vert   = verts;
  mesh.ntris  = ntris;
  mesh.tri    = tris;



  /*** create vertex array object ***/

  // create empty vertex array object
  glGenVertexArrays(1, &mesh.vao);
  glBindVertexArray(mesh.vao);



  /*** create vertex buffer object ***/

  // create empty vertex buffer object
  glGenBuffers(1, &mesh.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

  // define and set the vertex buffer
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(vertex_t) * nverts,
      mesh.vert,
      GL_STATIC_DRAW
  );

  // define vertex coordinates
  glVertexAttribPointer(
      0,                              // shader location defined in shader
      3,                              // three values per index
      GL_FLOAT,                       // each index value is a float
      GL_FALSE,                       // normalization
      sizeof(vertex_t),               // size of one index
      (void *)offsetof(vertex_t, pos) // struct offset
  );

  // enable this attribute with index 0
  glEnableVertexAttribArray(0);

  // define vertex uv
  glVertexAttribPointer(
      1,                  // shader location
      2,                  // two values: u and v
      GL_FLOAT,
      GL_FALSE,
      sizeof(vertex_t),
      (void *)(offsetof(vertex_t, uv))
  );

  // enable this attribute with index 1
  glEnableVertexAttribArray(1);



  /*** create index buffer object ***/
  
  // create index object
  glGenBuffers(1, &mesh.ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);

  // define and set index buffer
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(tri_t)*mesh.ntris,
      mesh.tri,
      GL_STATIC_DRAW
  );


  return mesh;
}

void draw_object(render_object_t *object, camera_t *camera){
    
  update_model(object);

  glUseProgram(object->shader.program);
  glBindVertexArray(object->mesh.vao);



  glUniformMatrix4fv(
    object->shader.projection,
    1,
    GL_FALSE,
    camera->projection.i
  ); 

  glUniformMatrix4fv(
      object->shader.model,
      1,
      GL_FALSE,
      object->model.i
  ); 

  glUniformMatrix4fv(
    object->shader.view, 
    1, 
    GL_FALSE, 
    camera->view.i
  );

  glDrawElements(
      GL_TRIANGLES,
      object->mesh.ntris * 3, //mul by 3 for # of idx per tri
      GL_UNSIGNED_INT,
      0
  );

}

void gl_error_check(void){
    // check for errors
    GLenum gl_err;
    gl_err = glGetError();
    while(gl_err != GL_NO_ERROR){
        logging.warn("render failure");
        gl_err = glGetError();
    }
}

void update_model(render_object_t *object){
    if (!object->remodel)
        return;

    mat4 model = mat4_identity();

    model = mat4_mul(model, mat4_translate_pos(object->pos));
    model = mat4_mul(model, mat4_rotate_y(object->rot.y));
    model = mat4_mul(model, mat4_rotate_x(object->rot.x));
    model = mat4_mul(model, mat4_rotate_z(object->rot.z));
    model = mat4_mul(model, mat4_scale(object->scale));

    object->model = model;
    object->remodel = false;
}

void update_camera(camera_t *camera){
  mat4 view;
  view = mat4_identity();
  view = mat4_mul(mat4_translate_camera(camera->pos), view);
  view = mat4_mul(mat4_rotate_y(camera->theta), view);
  view = mat4_mul(mat4_rotate_x(camera->phi), view);
  camera->view = view;
}

void hsv_to_rgb(float h, float s, float v,
                u8 *r, u8 *g, u8 *b)
{
    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float rf, gf, bf;

    if (h < 60) {
        rf = c; gf = x; bf = 0;
    } else if (h < 120) {
        rf = x; gf = c; bf = 0;
    } else if (h < 180) {
        rf = 0; gf = c; bf = x;
    } else if (h < 240) {
        rf = 0; gf = x; bf = c;
    } else if (h < 300) {
        rf = x; gf = 0; bf = c;
    } else {
        rf = c; gf = 0; bf = x;
    }

    *r = (u8)((rf + m) * 255.0f);
    *g = (u8)((gf + m) * 255.0f);
    *b = (u8)((bf + m) * 255.0f);
}

void create_palette(rgba_t *pixels){

  for (u32 y = 0; y < 256; y++) {
      for (u32 x = 0; x < 256; x++) {

          rgba_t *p = &pixels[y * 256 + x];

          if (y < 255) {
            float hue = 360.0f * x / 255.0f;
            float value = 1.0f - (float)y / 254.0f;

            hsv_to_rgb(
                hue,
                1.0f,
                value,
                &p->r,
                &p->g,
                &p->b
            );

            p->a = 255;
          } else {
            u8 c = x;

            p->r = c;
            p->g = c;
            p->b = c;
            p->a = 255;
          }
      }
  }
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

  player.pos = (vec3){ 0 , 2 , 0 };


  vertex_t *vertices = NULL;
  tri_t *triangles = NULL;

  // objects defined in header file
  demo_tri_
  render_object_t demo_tri;
  demo_tri.shader = init_shader("./src/render/basic.vert",
                                "./src/render/basic.frag");
  demo_tri.mesh   = init_mesh(3, vertices, 1, triangles);
  demo_tri.pos    = (vec3){ 0 , 1 ,-10};
  demo_tri.rot    = (vec3){ 0 , 0 , 0 };
  demo_tri.scale  = (vec3){ 1 , 1 , 1 };
  demo_tri.remodel  = 1;

  render_object_t demo_tri2;
  demo_tri2.shader = init_shader("./src/render/basic.vert",
                                "./src/render/basic.frag");
  demo_tri2.mesh   = init_mesh(3, vertices, 1, triangles);
  demo_tri2.pos    = (vec3){ 5 , 3 ,-8};
  demo_tri2.rot    = (vec3){ 1.0f , 1.0f , 1.0f };
  demo_tri2.scale  = (vec3){ 1 , 1 , 1 };
  demo_tri2.remodel  = 1;

  ground_

  tower_
  render_object_t tower;
  tower.shader = init_shader("./src/render/basic.vert",
                              "./src/render/basic.frag");
  tower.mesh = init_mesh(32, vertices, 48, triangles);
  tower.pos = (vec3){-20,0,-20};
  tower.rot    = (vec3){ 0 , 0 , 0 };
  tower.scale  = (vec3){ 1 , 1 , 1 };
  tower.remodel = 1;
  
  render_object_t tower2;
  tower2.shader = init_shader("./src/render/basic.vert",
                              "./src/render/basic.frag");
  tower2.mesh = init_mesh(32, vertices, 48, triangles);
  tower2.pos = (vec3){20,0,-20};
  tower2.rot    = (vec3){ 0 , 0 , 0 };
  tower2.scale  = (vec3){ .5 , .5 , .5 };
  tower2.remodel = 1;


  roof_

  render_object_t roof;                             
  roof.shader = init_shader("./src/render/basic.vert", 
                             "./src/render/basic.frag");  
  roof.mesh = init_mesh(9, vertices, 8, triangles); 
  roof.pos = (vec3){-20,15,-20};
  roof.rot    = (vec3){ 0 , 0 , 0 };
  roof.scale  = (vec3){ 1 , 1 , 1 };
  roof.remodel = 1;

  render_object_t roof2;                             
  roof2.shader = init_shader("./src/render/basic.vert", 
                             "./src/render/basic.frag");  
  roof2.mesh    = init_mesh(9, vertices, 8, triangles); 
  roof2.pos     = (vec3){20,7.5f,-20};
  roof2.rot    = (vec3){ 0 , 0 , 0 };
  roof2.scale  = (vec3){ .5 , .5 , .5 };
  roof2.remodel = 1;

  
  camera.projection = mat4_perspective(
      70.0f,        // field of view
      (float)gui.window.dim.w/(float)gui.window.dim.h, // aspect ratio
      0.1f,         // near plane
      1000.0f       // far plane
  );



  rgba_t *pixels = malloc(256 * 256 * sizeof(rgba_t));
  create_palette(pixels);

  GLuint texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);  

  glTexImage2D(
      GL_TEXTURE_2D,
      0,                  // mip level
      GL_RGBA8,           // GPU storage format
      256,
      256,
      0,                  // border (always 0)
      GL_RGBA,            // format of your CPU data
      GL_UNSIGNED_BYTE,   // each channel is a u8
      pixels
  );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  free(pixels);

  while(modes.RUNNING){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_camera(&camera);

    
    draw_object(&demo_tri, &camera);
    draw_object(&demo_tri2, &camera);

    draw_object(&ground, &camera);
    draw_object(&tower, &camera);
    draw_object(&roof, &camera);

    draw_object(&tower2, &camera);
    draw_object(&roof2, &camera);

    gl_error_check();

    SDL_GL_SwapWindow(gui.sdl2.window);

  }


  SDL_DestroyWindow(gui.sdl2.window);
  SDL_Quit();
}