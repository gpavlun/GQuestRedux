// clang-format off
#include "render.h"

#include <pthread.h>

#include "../rendering/obj_parser.h"
#include "../main/game.h"
#include "../rendering/lighting.h"

/***** load_shader *****/
void gpu_upload_shader(shader_t *shader){
  if (!shader) {
    logging.warn("render_thread: shader invalid for load");
    return;
  }

  GLint gl_success_code;
  /*** create vertex shader ***/

  //logging.info("render_thread: uploading .vert");
  // read in shader data and create a context for it
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  logging.assert(vertex_shader, "GL vertex failed!");

  // load shader source
  //logging.info("render_thread: loading .vert source");
  glShaderSource(vertex_shader, 1, (const GLchar * const*)&shader->vert_glsl, NULL);

  // compile shader source
  //logging.info("render_thread: compiling .vert source");
  glCompileShader(vertex_shader);
  
  // check for compile failure
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetShaderInfoLog(vertex_shader, 512, NULL, log);
    logging.error(0, log);
  }

  /*** create fragment shader ***/

  //logging.info("render_thread: uploading .frag");
  // read in shader data and create a context for it
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  //logging.assert(fragment_shader, "GL fragment failed!");

  // load shader source
  //logging.info("render_thread: loading .frag source");
  glShaderSource(fragment_shader, 1, (const GLchar * const*)&shader->frag_glsl, NULL);

  // compile shader source
  //logging.info("render_thread: compiling .frag source");
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
  //logging.info("render_thread: creating shader program");
  shader->program = glCreateProgram();
  //logging.assert(shader->program, "GL program failed!");

  // attach the shaders to the object
  //logging.info("render_thread: attaching shader vert");
  glAttachShader(shader->program, vertex_shader);
  //logging.info("render_thread: attaching shader frag");
  glAttachShader(shader->program, fragment_shader);

  // finish the linking
  //logging.info("render_thread: linking shader program");
  glLinkProgram(shader->program);

  // check for linking failure
  glGetProgramiv(shader->program, GL_LINK_STATUS, &gl_success_code);
  if(!gl_success_code){
    char log[512];
    glGetProgramInfoLog(shader->program, 512, NULL, log);
    logging.error(0, log);
  }

  /*** variable declarations for program ***/

  //logging.info("render_thread: declaring shader variables");
  shader->projection = glGetUniformLocation(shader->program, "projection");
  logging.assert(shader->projection != -1, "render_thread: projection not found");

  shader->view = glGetUniformLocation(shader->program, "view");
  logging.assert(shader->view != -1, "render_thread: projection not found");

  shader->model = glGetUniformLocation(shader->program, "model");
  logging.assert(shader->model != -1, "render_thread: model not found");

  free(shader->vert_glsl);
  free(shader->frag_glsl);
  shader->vert_glsl = NULL;
  shader->frag_glsl = NULL;
}

/***** load_mesh *****/
void gpu_upload_mesh(mesh_t *mesh){
  if(!mesh){
    logging.warn("render_thread: mesh invalid for load");
    return;
  }

  /*** create objects ***/

  // create empty vertex array object
  glGenVertexArrays(1, &mesh->vao);
  //logging.data("generated vao", mesh->vao);
  glBindVertexArray(mesh->vao);

  // create empty vertex buffer object
  glGenBuffers(1, &mesh->vbo);
  //logging.data("generated vbo", mesh->vbo);
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
  //logging.data("generated ebo", mesh->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);

  // define and set index buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tri_t)*mesh->ntris,
               mesh->tri, GL_STATIC_DRAW);

  free(mesh->tri);
  free(mesh->vert);
  mesh->tri = NULL;
  mesh->vert = NULL;
}
// #ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
// #define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
// #endif

// #ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
// #define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
// #endif
/***** load_texture *****/
void gpu_upload_texture(texture_t *texture) {
  GLuint handle;

  glGenTextures(1, &handle);
  glBindTexture(GL_TEXTURE_2D, handle);

  glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGB8,
      texture->width,
      texture->height,
      0,
      GL_RGB,
      GL_UNSIGNED_BYTE,
      texture->pixels);

  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // float maxAniso = 0.0f;

  // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

  // glTexParameterf(GL_TEXTURE_2D,
  //                 GL_TEXTURE_MAX_ANISOTROPY_EXT,
  //                 maxAniso);

  texture->handle = handle;
  free(texture->pixels);
  texture->pixels = NULL;
}

/***** draw_terrain *****/
void draw_terrain(
  logical_chunk_t *terrain,
  camera_t *camera,
  lighting_t *lighting,
  mesh_table_t *mesh_table,
  shader_table_t *shader_table,
  texture_table_t *texture_table
){

  size_t mesh_idx = terrain->render_data.mesh_idx;
  size_t shader_idx = terrain->render_data.shader_idx;
  size_t texture_idx = terrain->render_data.texture_idx;

  shader_t *shaders = shader_table->shader;
  mesh_t *meshes = mesh_table->mesh;
  texture_t *textures = texture_table->texture;

  terrain_update_model(terrain);


  glUseProgram(shaders[shader_idx].program);

  glBindVertexArray(meshes[mesh_idx].vao);

  glUniformMatrix4fv(shaders[shader_idx].projection, 1, GL_FALSE, camera->projection.i);

  glUniformMatrix4fv(shaders[shader_idx].model, 1, GL_FALSE, terrain->render_data.model.i);

  glUniformMatrix4fv(shaders[shader_idx].view, 1, GL_FALSE, camera->view.i);

  glUniform3fv(glGetUniformLocation(shaders[shader_idx].program, "light_ray_direction"), 1, &lighting->direction.x);

  glUniform1f(glGetUniformLocation(shaders[shader_idx].program, "ambient_light"), lighting->ambient);

  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, textures[texture_idx].handle);

  glUniform1i(glGetUniformLocation(shaders[shader_idx].program, "palette"), 0);

  //mul by 3 for # of idx per tri
  glDrawElements(GL_TRIANGLES, (int)meshes[mesh_idx].ntris * 3, GL_UNSIGNED_INT, 0);


}

/***** draw_entity *****/
void draw_entity(
  entity_t *entity,
  camera_t *camera,
  lighting_t *lighting,
  mesh_table_t *mesh_table,
  shader_table_t *shader_table,
  texture_table_t *texture_table
){
  if (entity->render_data.hidden) return;

  size_t mesh_idx = entity->render_data.mesh_idx;
  size_t shader_idx = entity->render_data.shader_idx;
  size_t texture_idx = entity->render_data.texture_idx;

  shader_t *shaders = shader_table->shader;
  mesh_t *meshes = mesh_table->mesh;
  texture_t *textures = texture_table->texture;

  update_model(entity);


  glUseProgram(shaders[shader_idx].program);

  glBindVertexArray(meshes[mesh_idx].vao);

  glUniformMatrix4fv(shaders[shader_idx].projection, 1, GL_FALSE, camera->projection.i);

  glUniformMatrix4fv(shaders[shader_idx].model, 1, GL_FALSE, entity->render_data.model.i);

  glUniformMatrix4fv(shaders[shader_idx].view, 1, GL_FALSE, camera->view.i);

  glUniform3fv(glGetUniformLocation(shaders[shader_idx].program, "light_ray_direction"), 1, &lighting->direction.x);

  glUniform1f(glGetUniformLocation(shaders[shader_idx].program, "ambient_light"), lighting->ambient);

  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, textures[texture_idx].handle);

  glUniform1i(glGetUniformLocation(shaders[shader_idx].program, "palette"), 0);

  //mul by 3 for # of idx per tri
  glDrawElements(GL_TRIANGLES, (int)meshes[mesh_idx].ntris * 3, GL_UNSIGNED_INT, 0);


}

void gl_error_check(void){
    GLenum gl_err;
    gl_err = glGetError();
    while(gl_err != GL_NO_ERROR){
        logging.warn("render_thread: render failure");
        logging.data("render_thread: error code", gl_err);
        gl_err = glGetError();
    }
}

void terrain_update_model(logical_chunk_t *terrain){
    if (!terrain->render_data.remodel)
        return;

    mat4 model = mat4_identity();

    model = mat4_mul(model, mat4_translate_pos(terrain->pos));

    terrain->render_data.model   = model;
    terrain->render_data.remodel = false;
}

void update_model(entity_t *entity){
    if (!entity->render_data.remodel)
        return;

    mat4 model = mat4_identity();

    model = mat4_mul(model, mat4_translate_pos(entity->transform.pos));
    model = mat4_mul(model, mat4_rotate_y(entity->transform.rot.y));
    model = mat4_mul(model, mat4_rotate_x(entity->transform.rot.x));
    model = mat4_mul(model, mat4_rotate_z(entity->transform.rot.z));
    model = mat4_mul(model, mat4_scale(entity->transform.scale));

    entity->render_data.model   = model;
    entity->render_data.remodel = false;
}

void update_camera(camera_t *camera){
  mat4 view;
  view = mat4_identity();

  view = mat4_mul(mat4_translate_camera(camera->pos), view);
  view = mat4_mul(mat4_rotate_z(camera->rot.z), view);
  view = mat4_mul(mat4_rotate_y(camera->rot.y), view);
  view = mat4_mul(mat4_rotate_x(camera->rot.x), view);

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
        float hue = 360.0f * (float)x / 255.0f;
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












void create_glctx(SDL_Window *window, SDL_GLContext *context) {
  // create the gpu connection
  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  logging.assert(
    gl_context != NULL,
    "SDL+GL context failed"
  );

  // assign this thread as the renderer
  logging.assert(
    SDL_GL_MakeCurrent(window, gl_context) == 0,
    "SDL+GL window selection failed"
  );

  // bind the gpu functions to this script
  logging.assert(
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress),
    "render_thread: GLAD failed"
  );

  *context = gl_context;

}


void render_loop(
  simulation_t *simulation, 
  mesh_table_t *mesh_table,
  shader_table_t *shader_table,
  texture_table_t *texture_table,
  gui_data_t *gui,
  boolean_t *modes
) {

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);      // Optional; this is the default.
  glFrontFace(GL_CCW);


  create_palette();

  project_camera(gui, &simulation->player_table[0].camera);

  sun_t sun = new_sun();
  lighting_t lighting = {0};

  lighting.ambient = .5f;

  bool setting = ! modes->wireframe;
  static uint64_t frame = 0;
  logging.data("render entities:", simulation->nentities);
  size_t count;
  while(modes->running){

    if ((frame++ % 300) == 0) {
      logging.data("render frame", frame);
    }

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
      simulation->player_table[0].camera.projection = mat4_perspective(70.0f, aspect, 0.1f, 1000.0f);
    }


    // move to sim
    update_camera(&simulation->player_table[0].camera);
    update_lighting(&lighting, &sun);



    if(modes->wireframe) lighting.ambient = 1.0f;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    count = simulation->nterrains;
    for (int i=0; i<count; i++) {
      draw_terrain(
        &simulation->terrain_table[i], 
        &simulation->player_table[0].camera, 
        &lighting,
        mesh_table,
        shader_table,
        texture_table
      );
    }

    count = simulation->nentities;
    for (int i=0; i<count; i++) {
      draw_entity(
        &simulation->entity_table[i], 
        &simulation->player_table[0].camera, 
        &lighting,
        mesh_table,
        shader_table,
        texture_table
      );
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
  logging.info("render_thread: starting");
  engine_t *engine = (engine_t *)arg;
  gui_data_t *gui = &engine->gui;
  simulation_t *simulation = &engine->simulation;

  mesh_table_t *mesh_table = &engine->mesh_table;
  shader_table_t *shader_table = &engine->shader_table;
  texture_table_t *texture_table = &engine->texture_table;

  boolean_t *modes = &engine->modes;
  pthread_barrier_t *barrier = &engine->barrier;
  logging.info("render_thread: caches created");

  logging.info("render_thread: creating context...");
  create_glctx(gui->window.interface, &engine->gl_context);
  logging.info("render_thread: context created");

  size_t count;

  logging.info("render_thread: creating meshes...");
  count = mesh_table->nmeshes;
  for (size_t i=0; i<count; i++)
    gpu_upload_mesh(&mesh_table->mesh[i]);
  logging.info("render_thread: meshes created");

  logging.info("render_thread: creating textures...");
  count = texture_table->ntextures;
  for (size_t i=0; i<count; i++)
    gpu_upload_texture(&texture_table->texture[i]);
  logging.info("render_thread: textures created");

  logging.info("render_thread: creating shaders...");
  count = shader_table->nshaders;
  for (size_t i=0; i<count; i++)
    gpu_upload_shader(&shader_table->shader[i]);
  logging.info("render_thread: shaders created");


  /********* THE GREAT WALL *********/
  logging.info("render_thread: render init finished, waiting at barrier");
  pthread_barrier_wait(barrier);
  logging.info("render_thread: render loop starting");
  /**********************************/

  render_loop(
    simulation,
    mesh_table,
    shader_table,
    texture_table,
    gui,
    modes
  );


  SDL_DestroyWindow(gui->window.interface);
  SDL_Quit();

  pthread_exit(NULL);
  return NULL;
}