// clang-format off
#include "render.h"

#include "obj_parser.h"


/***** load_shader *****/
void gpu_upload_shader(shader_t *shader){
  if (!shader) {
    logging.warn("shader invalid for load");
    return;
  }
  GLint gl_success_code;
  /*** create vertex shader ***/

  // read in shader data and create a context for it
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  logging.assert(vertex_shader, "GL vertex failed!");

  // load shader source
  glShaderSource(vertex_shader, 1, (const GLchar * const*)shader->vert_glsl, NULL);

  // compile shader source
  glCompileShader(vertex_shader);
  
  // check for compile failure
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetShaderInfoLog(vertex_shader, 512, NULL, log);
    logging.error(0, log);
  }

  /*** create fragment shader ***/

  // read in shader data and create a context for it
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  logging.assert(fragment_shader, "GL fragment failed!");

  // load shader source
  glShaderSource(fragment_shader, 1, (const GLchar * const*)shader->frag_glsl, NULL);

  // compile shader source
  glCompileShader(fragment_shader);

  // check for compile failure
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetShaderInfoLog(fragment_shader, 512, NULL, log);
    logging.error(0, log);
  }

  /*** link the vertex and fragment shaders ***/

  // create a program object
  shader->program = glCreateProgram();
  logging.assert(shader->program, "GL program failed!");

  // attach the shaders to the object
  glAttachShader(shader->program, vertex_shader);
  glAttachShader(shader->program, fragment_shader);

  // finish the linking
  glLinkProgram(shader->program);

  // check for linking failure
  glGetProgramiv(shader->program, GL_LINK_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetProgramInfoLog(shader->program, 512, NULL, log);
    logging.error(0, log);
  }

  /*** variable declarations for program ***/

  shader->projection = glGetUniformLocation(shader->program, "projection");
  logging.assert(shader->projection != -1, "projection not found");

  shader->view = glGetUniformLocation(shader->program, "view");
  logging.assert(shader->view != -1, "projection not found");

  shader->model = glGetUniformLocation(shader->program, "model");
  logging.assert(shader->model != -1, "model not found");

  free(shader->vert_glsl);
  free(shader->frag_glsl);
  shader->vert_glsl = NULL;
  shader->frag_glsl = NULL;
}

/***** load_mesh *****/
void gpu_upload_mesh(mesh_t *mesh){
  if(!mesh){
    logging.warn("mesh invalid for load");
    return;
  }

  /*** create objects ***/

  // create empty vertex array object
  glGenVertexArrays(1, &mesh->vao);
  glBindVertexArray(mesh->vao);

  // create empty vertex buffer object
  glGenBuffers(1, &mesh->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

  // define and set the vertex buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * mesh->nverts,
               mesh->vert, GL_STATIC_DRAW);

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

  // define vertex normal
  glVertexAttribPointer(
      1,                  // shader location
      3,                  // three values: x, y, and z
      GL_FLOAT,
      GL_FALSE,
      sizeof(vertex_t),
      (void *)(offsetof(vertex_t, normal))
  );

  // enable this attribute with index 1
  glEnableVertexAttribArray(1);

  // define vertex uv
  glVertexAttribPointer(
      2,                  // shader location
      2,                  // two values: u and v
      GL_FLOAT,
      GL_FALSE,
      sizeof(vertex_t),
      (void *)(offsetof(vertex_t, uv))
  );

  // enable this attribute with index 2
  glEnableVertexAttribArray(2);

  /*** create index buffer object ***/
  
  // create index object
  glGenBuffers(1, &mesh->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);

  // define and set index buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tri_t)*mesh->ntris,
               mesh->tri, GL_STATIC_DRAW);

  free(mesh->tri);
  free(mesh->vert);
  mesh->tri = NULL;
  mesh->vert = NULL;
}

/***** draw_object *****/
void draw_object(render_object_t *object, camera_t *camera, lighting_t *lighting){
  if (object->hidden) return;

  update_model(object);

  glUseProgram(object->shader->program);
  glBindVertexArray(object->mesh->vao);

  glUniformMatrix4fv(object->shader->projection, 1, GL_FALSE, camera->projection.i);

  glUniformMatrix4fv(object->shader->model, 1, GL_FALSE, object->model.i);

  glUniformMatrix4fv(object->shader->view, 1, GL_FALSE, camera->view.i);

  glUniform3fv(glGetUniformLocation(object->shader->program, "light_ray_direction"), 1, &lighting->direction.x);

  glUniform1f(glGetUniformLocation(object->shader->program, "ambient_light"), lighting->ambient);

  glUniform1i(glGetUniformLocation(object->shader->program, "palette"), 0);

  //mul by 3 for # of idx per tri
  glDrawElements(GL_TRIANGLES, object->mesh->ntris * 3, GL_UNSIGNED_INT, 0);

}

void gl_error_check(void){
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
  view = mat4_mul(mat4_rotate_y(camera->rot.y), view);
  view = mat4_mul(mat4_rotate_x(camera->rot.x), view);
  view = mat4_mul(mat4_rotate_z(camera->rot.z), view);
  camera->view = view;
}

void hsv_to_rgb(float h, float s, float v, u8 *r, u8 *g, u8 *b){
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

void create_palette(void){
  rgba_t *pixels = malloc(256 * 256 * sizeof(rgba_t));

  for (u32 y = 0; y < 256; y++) {
    for (u32 x = 0; x < 256; x++) {
      rgba_t *p = &pixels[y * 256 + x];

      if (y < 255) {
        float hue = 360.0f * x / 255.0f;
        float value = 1.0f - (float)y / 254.0f;
        hsv_to_rgb(hue, 1.0f, value, &p->r, &p->g, &p->b);
        p->a = 255;
      }else{
        p->r = x;
        p->g = x;
        p->b = x;
        p->a = 255;
      }
    }
  }

  GLuint texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  free(pixels);
}











void project_camera(gui_data_t *gui, camera_t *camera) {
  camera->projection = mat4_perspective(
      70.0f,        // field of view
      (float)gui->window.dim.w/(float)gui->window.dim.h, // aspect ratio
      0.1f,         // near plane
      1000.0f       // far plane
  );
}












void create_glctx(SDL_Window *window) {
  // create the gpu connection
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  // bind the gpu functions to this script
  logging.assert(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "GLAD failed");
}


void render_loop(render_state_t *render, gui_data_t *gui, render_array_t render_array) {
  glEnable(GL_DEPTH_TEST);

  create_palette();

  project_camera(gui, &render->camera);

  sun_t sun = new_sun();
  lighting_t lighting = {0};

  lighting.ambient = .5f;

  boolean_t *modes = render->modes;
  bool setting = ! modes->wireframe;

  while(modes->running){



    if(setting !=  modes->wireframe){
      if(modes->wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor(0.486f, 0.686f, 0.761f, 1.0f);
      }
      setting =  modes->wireframe;
    }

    if (get_dim(&gui->window)) {
      glViewport(0, 0, gui->window.dim.w, gui->window.dim.h);
      float aspect = (float)gui->window.dim.w / (float)gui->window.dim.h;
      render->camera.projection = mat4_perspective(70.0f, aspect, 0.1f, 1000.0f);
    }


    // move to sim
    update_camera(&render->camera);
    update_lighting(&lighting, &sun);



    if(modes->wireframe) lighting.ambient = 1.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_array.object[4]->pos = render->player_transform.pos;
    render_array.object[4]->remodel = 1;
    for (int i=0; i<render_array.nobjects; i++) {
      draw_object(render_array.object[i], &render->camera, &lighting);
    }


    gl_error_check();
    SDL_GL_SwapWindow(gui->window.interface);
  }
}




/*
 * The renderer realistically needs to have:
 *  1. sdl window context
 *  2. camera position
 *  3. entity positions
 */
void *start_render(void *arg){
  //simulation_t *simulation, mesh_t *mesh_table, pthread_barrier_t *barrier
  engine_t *engine = (engine_t*)arg;
  gui_data_t *gui = &engine->gui;
  simulation_t *simulation = &engine->simulation;
  mesh_table_t *mesh_table = &engine->mesh_table;
  pthread_barrier_t *barrier = &engine->barrier;

  create_glctx(gui->window.interface);

  shader_t *shared_shader = malloc(sizeof(shader_t));
  *shared_shader = init_shader("./src/render/basic.vert", "./src/render/basic.frag");

  size_t nmeshes = mesh_table->nmeshes;
  for (size_t i=0; i<nmeshes; i++) {
    gpu_upload_mesh(&mesh_table->mesh[i]);
  }

  // shader table like above


  /********* THE GREAT WALL *********/
  logging.info("render init finished");
  pthread_barrier_wait(barrier);
  logging.info("render loop starting");
  /**********************************/



  //render_loop(&engine->render, &engine->gui, render_array);


  SDL_DestroyWindow(gui->window.interface);
  SDL_Quit();

  pthread_exit(NULL);
  return NULL;
}