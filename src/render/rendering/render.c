// clang-format off
#include "render.h"

#include "obj_parser.h"


logical_chunk_t *glob_terrain;

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

void mesh_generate_normals(mesh_t *mesh){
  if(!mesh) return;
  
  u32 i;

  for(i=0;i<mesh->nverts;i++){
    mesh->vert[i].normal = (vec3){0,0,0};
  }

  for(i=0;i<(mesh->ntris);i++){

    u32 ia = mesh->tri[i].a;
    u32 ib = mesh->tri[i].b;
    u32 ic = mesh->tri[i].c;

    vec3 a = mesh->vert[ia].pos;
    vec3 b = mesh->vert[ib].pos;
    vec3 c = mesh->vert[ic].pos;

    vec3 edge1 = vec3_sub(b,a);
    vec3 edge2 = vec3_sub(c,a);

    vec3 normal = vec3_cross(edge1,edge2);
    normal = vec3_normalize(normal);

    mesh->vert[ia].normal =
      vec3_add(mesh->vert[ia].normal,normal);

    mesh->vert[ib].normal =
      vec3_add(mesh->vert[ib].normal,normal);

    mesh->vert[ic].normal =
      vec3_add(mesh->vert[ic].normal,normal);
  }

  for(i=0;i<mesh->nverts;i++){
    mesh->vert[i].normal =
      vec3_normalize(mesh->vert[i].normal);
  }
}


mesh_t new_mesh(size_t nverts, vertex_t *verts, size_t ntris, tri_t *tris) {
  mesh_t mesh = {0};

  mesh.nverts = nverts;
  mesh.vert   = verts;
  mesh.ntris  = ntris;
  mesh.tri    = tris;

  return mesh;
}

void load_mesh(mesh_t *mesh){
  if(!mesh) return;


  /*** create vertex array object ***/

  // create empty vertex array object
  glGenVertexArrays(1, &mesh->vao);
  glBindVertexArray(mesh->vao);



  /*** create vertex buffer object ***/

  // create empty vertex buffer object
  glGenBuffers(1, &mesh->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);

  // define and set the vertex buffer
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(vertex_t) * mesh->nverts,
      mesh->vert,
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
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER,
      sizeof(tri_t)*mesh->ntris,
      mesh->tri,
      GL_STATIC_DRAW
  );

}

void draw_object(render_object_t *object, camera_t *camera, lighting_t *lighting){
    
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
  view = mat4_mul(mat4_rotate_y(camera->theta), view);
  view = mat4_mul(mat4_rotate_x(camera->phi), view);
  view = mat4_mul(mat4_rotate_z(camera->psi), view);
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


  float height_func(float x, float z){
      return 
        sinf(x * 0.08f) * 3.0f +
        cosf(z * 0.08f) * 3.0f;

  }

void rotate_object(render_object_t *object) {

  static u32 old;
  u32 now = SDL_GetTicks();
  float elapsed = now - old;
  if (elapsed > 10) {
    old = now;
    object->rot.y += 0.1f;
    if (object->rot.y > 6.28) object->rot.y = 0;
    object->remodel = true;
  }
}



logical_chunk_t generate_terrain(){
    i32 r, c, t;
    
    logical_chunk_t logical_chunk = {0};

    logical_chunk.object.pos = (vec3){0,0,0};
    logical_chunk.heightmap = calloc($chunk_verts*$chunk_verts, sizeof(float));
    for(r=0; r<$chunk_verts; r++){
    for(c=0; c<$chunk_verts; c++){

      *(logical_chunk.heightmap + r*$chunk_verts + c) = 
      height_func(r - $chunk_verts/2, c - $chunk_verts/2);

    }
    }
    // flat spot
    #define $flat_w 85
    for(r=0;r<$flat_w;r++){
    for(c=0;c<$flat_w;c++){
      *(logical_chunk.heightmap + ($chunk_verts/2+$flat_w/2+100-r)*$chunk_verts + ($chunk_verts/2+$flat_w/2-100-c)) = 0.0f;
    }
    }


    vertex_t *vertices = NULL;
    tri_t *triangles = NULL;

    vertices  = calloc($chunk_verts * $chunk_verts, sizeof(vertex_t));
    triangles = calloc($chunk_quads * $chunk_quads* 2, sizeof(tri_t));

    for(r=0; r<$chunk_verts; r++){
    for(c=0; c<$chunk_verts; c++){
        (vertices + r*$chunk_verts + c)->pos = 
          (vec3){
            c - $chunk_verts/2,
            logical_chunk.heightmap[r*$chunk_verts+c],
            r - $chunk_verts/2
          };

          (vertices + r*$chunk_verts + c)->uv = (vec2){0.33f, 0.7f};
    }
    }




    t=0;
    for(r = 0; r < $chunk_quads; r++){
    for(c = 0; c < $chunk_quads; c++){

        u32 a = r * $chunk_verts + c;
        u32 b = r * $chunk_verts + c + 1;
        u32 c_idx = (r + 1) * $chunk_verts + c;
        u32 d = (r + 1) * $chunk_verts + c + 1;

        triangles[t++] = (tri_t){a, c_idx, b};
        triangles[t++] = (tri_t){b, c_idx, d};
    }
    }


    logging.assert(t == $chunk_quads * $chunk_quads * 2,
                   "terrain failure, quads misaligned!");

    logical_chunk.object.mesh = calloc(1, sizeof(mesh_t));
    *logical_chunk.object.mesh = new_mesh($chunk_verts*$chunk_verts, vertices,
                          $chunk_quads*$chunk_quads*2, triangles);

    return logical_chunk;
  }


void render_array_add(render_array_t *array, render_object_t *object) {
  array->nobjects++;
  array->object = realloc(array->object, sizeof(render_object_t *) * (array->nobjects));
  array->object[array->nobjects - 1] = object;
}

// lets add a table of all the meshes we need, then have it return handles with string look ups

void load_terrain(render_array_t *array) {
  //terrain gen:
  logical_chunk_t *terrain = calloc(1, sizeof(logical_chunk_t));
  *terrain = generate_terrain();
  glob_terrain = terrain;

  mesh_generate_normals(terrain->object.mesh);
  load_mesh(terrain->object.mesh);
  terrain->object.shader = malloc(sizeof(shader_t));
  *terrain->object.shader = init_shader("./src/render/basic.vert", "./src/render/basic.frag");
  terrain->object.remodel = 1;
  terrain->object.pos    = (vec3){ 0 , 0 , 0};
  terrain->object.rot    = (vec3){ 0 , 0 , 0 };
  terrain->object.scale  = (vec3){ 1 , 1 , 1 };

  render_array_add(array, &terrain->object);
}

void instantiate_object(char *path) {
  mesh_t object_mesh = obj_from_file("./assets/barad_dur.obj");
  mesh_generate_normals(&object_mesh);
  for (int i=0; i<object_mesh.nverts; i++) {
    object_mesh.vert[i].uv = (vec2){0.1f, 0.999f};
  }

  load_mesh(&object_mesh);

  render_object_t barad_dur;
  barad_dur.mesh   = &object_mesh;
  //barad_dur.shader = NULL;
  barad_dur.pos    = (vec3){ 0 , 0 ,-60};
  barad_dur.pos.y  = terrain_height(barad_dur.pos) - 2.0f;
  barad_dur.rot    = (vec3){ -1.57f , 0 , 0 };
  barad_dur.scale  = (vec3){ 1 , 1 , 1 };
  barad_dur.remodel  = 1;
}


void create_all_temp(render_array_t *array, shader_t *shared_shader) {
  vertex_t *vertices = NULL;
  tri_t *triangles = NULL;

  // demo triangle
  demo_tri_;
  mesh_t *demo_tri_mesh = malloc(sizeof(mesh_t));
  *demo_tri_mesh = new_mesh(3, vertices, 1, triangles);
  mesh_generate_normals(demo_tri_mesh);
  load_mesh(demo_tri_mesh);

  render_object_t *demo_tri = malloc(sizeof(render_object_t));
  demo_tri->mesh = demo_tri_mesh;
  demo_tri->shader = shared_shader;
  demo_tri->pos    = (vec3){ 0 , 0 ,-25};
  demo_tri->pos.y  = terrain_height(demo_tri->pos) + 1.0f;
  demo_tri->rot    = (vec3){ 0 , 0 , 0 };
  demo_tri->scale  = (vec3){ 1 , 1 , 1 };
  demo_tri->remodel  = 1;
  render_array_add(array, demo_tri);

  // barad dur
  mesh_t *barad_dur_mesh = malloc(sizeof(mesh_t));
  *barad_dur_mesh = obj_from_file("./assets/barad_dur.obj");
  mesh_generate_normals(barad_dur_mesh);
  for (int i=0; i<barad_dur_mesh->nverts; i++) {
    barad_dur_mesh->vert[i].uv = (vec2){0.1f, 0.999f};
  }
  load_mesh(barad_dur_mesh);

  render_object_t *barad_dur = malloc(sizeof(render_object_t));
  barad_dur->mesh   = barad_dur_mesh;
  barad_dur->shader = shared_shader;
  barad_dur->pos    = (vec3){ 0 , 0 ,-60};
  barad_dur->pos.y  = terrain_height(barad_dur->pos) - 2.0f;
  barad_dur->rot    = (vec3){ -1.57f , 0 , 0 };
  barad_dur->scale  = (vec3){ 1 , 1 , 1 };
  barad_dur->remodel  = 1;
  render_array_add(array, barad_dur);

  mesh_t *eye_mesh = malloc(sizeof(mesh_t));
  *eye_mesh = obj_from_file("./assets/eye.obj");
  mesh_generate_normals(eye_mesh);
  load_mesh(eye_mesh);

  render_object_t *eye = malloc(sizeof(render_object_t));
  eye->mesh   = eye_mesh;
  eye->shader = shared_shader;
  eye->pos    = barad_dur->pos;
  eye->rot    = (vec3){ -1.57f , 0 , 0 };
  eye->scale  = (vec3){ 1 , 1 , 1 };
  eye->remodel  = 1;
  render_array_add(array, eye);


  // player
  mesh_t *player_mesh = malloc(sizeof(mesh_t));
  *player_mesh = obj_from_file("./assets/player.obj");
  mesh_generate_normals(player_mesh);
  load_mesh(player_mesh);

  render_object_t *player = malloc(sizeof(render_object_t));
  player->mesh   = player_mesh;
  player->shader = shared_shader;
  player->pos    = (vec3){0,0,0};
  player->rot    = (vec3){ -1.57f , 0 , 0 };
  player->scale  = (vec3){ 1 , 1 , 1 };
  player->remodel  = 1;
  render_array_add(array, player);

  // towers
  tower_
  mesh_t *tower_mesh = malloc(sizeof(mesh_t));
  *tower_mesh = new_mesh(32, vertices, 48, triangles);
  mesh_generate_normals(tower_mesh);
  load_mesh(tower_mesh);

  render_object_t *tower0 = malloc(sizeof(render_object_t));
  tower0->mesh   = tower_mesh;
  tower0->shader = shared_shader;
  tower0->pos    = (vec3){-20, 0,-20};
  tower0->pos.y  = terrain_height(tower0->pos) - 2.0f;
  tower0->rot    = (vec3){ 0 , 0 , 0 };
  tower0->scale  = (vec3){ 1 , 1 , 1 };
  tower0->remodel = 1;
  render_array_add(array, tower0);

  render_object_t *tower1 = malloc(sizeof(render_object_t));
  tower1->mesh   = tower_mesh;
  tower1->shader = shared_shader;
  tower1->pos    = (vec3){ 20, 0, -20};
  tower1->pos.y  = terrain_height(tower1->pos) - 1.0f;
  tower1->rot    = (vec3){ 0 , 0 , 0 };
  tower1->scale  = (vec3){ .5f , .5f , .5f };
  tower1->remodel = 1;
  render_array_add(array, tower1);


  // roofs
  roof_
  mesh_t *roof_mesh = malloc(sizeof(mesh_t));
  *roof_mesh = new_mesh(9, vertices, 8, triangles);
  mesh_generate_normals(roof_mesh);
  load_mesh(roof_mesh);


  render_object_t *roof0 = malloc(sizeof(render_object_t));
  roof0->mesh   = roof_mesh;
  roof0->shader = shared_shader;
  roof0->pos    = (vec3){-20,(tower0->pos.y + 15.0f),-20};
  roof0->rot    = (vec3){ 0 , 0 , 0 };
  roof0->scale  = (vec3){ 1 , 1 , 1 };
  roof0->remodel = 1;
  render_array_add(array, roof0);

  render_object_t *roof1 = malloc(sizeof(render_object_t));
  roof1->mesh   = roof_mesh;
  roof1->shader = shared_shader;
  roof1->pos    = (vec3){20,(tower1->pos.y + 7.5f),-20};
  roof1->rot    = (vec3){ 0 , 0 , 0 };
  roof1->scale  = (vec3){ .5f , .5f , .5f };
  roof1->remodel = 1;
  render_array_add(array, roof1);

  //castle
  mesh_t *castle_mesh = malloc(sizeof(mesh_t));
  *castle_mesh = obj_from_file("./assets/castle.obj");
  for (int i=0; i<castle_mesh->nverts; i++) {
    castle_mesh->vert[i].uv = (vec2){0.3f, 0.999f};
  }
  mesh_generate_normals(castle_mesh);
  load_mesh(castle_mesh);

  render_object_t *castle = malloc(sizeof(render_object_t));
  castle->mesh   = castle_mesh;
  castle->shader = shared_shader;
  castle->pos    = (vec3){-105,0,+100};
  castle->pos.y  = terrain_height(castle->pos) - 0.0f;
  castle->rot    = (vec3){ -1.57f , -1.57f , 0 };
  castle->scale  = (vec3){ 1 , 1 , 1 };
  castle->remodel  = 1;
  render_array_add(array, castle);

  mesh_t *castle_roof_mesh = malloc(sizeof(mesh_t));
  *castle_roof_mesh = obj_from_file("./assets/roofs.obj");
  for (int i=0; i<castle_roof_mesh->nverts; i++) {
    castle_roof_mesh->vert[i].uv = (vec2){0.078f,0.647f};
  }
  mesh_generate_normals(castle_roof_mesh);
  load_mesh(castle_roof_mesh);

  render_object_t *castle_roof = malloc(sizeof(render_object_t));
  castle_roof->mesh   = castle_roof_mesh;
  castle_roof->shader = shared_shader;
  castle_roof->pos    = castle->pos;
  castle_roof->pos.y  = terrain_height(castle_roof->pos) - 1.0f;
  castle_roof->rot    = (vec3){ -1.57f , -1.57f , 0 };
  castle_roof->scale  = (vec3){ 1 , 1 , 1 };
  castle_roof->remodel  = 1;
  render_array_add(array, castle_roof);

}

void init_cam(gui_engine_t *gui) {
  camera_glob->projection = mat4_perspective(
      70.0f,        // field of view
      (float)gui->window.dim.w/(float)gui->window.dim.h, // aspect ratio
      0.1f,         // near plane
      1000.0f       // far plane
  );

}























void start_render(void) {
  gui_engine_t gui = gui_engine_init();
  init_events(&gui);
  window_t *window = &gui.window;

  // create the gpu connection
  SDL_GLContext gl_context = SDL_GL_CreateContext(gui.sdl2.window);
  // bind the gpu functions to this script               
  logging.assert(gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress), "GLAD failed");

  glEnable(GL_DEPTH_TEST);
  render_array_t render_array = {0};
  load_terrain(&render_array);

  shader_t shared_shader = init_shader("./src/render/basic.vert", "./src/render/basic.frag");
  create_all_temp(&render_array, &shared_shader);



  init_cam(&gui);
  create_palette();

  sun_t sun = new_sun();

  lighting_t lighting = {0};
  lighting.ambient = .5f;

  while (!player_glob);

  bool setting = ! modes.WIREFRAME;
  while(modes.RUNNING){

    if(setting !=  modes.WIREFRAME){
      if(modes.WIREFRAME){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      }else{
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor(0.486f, 0.686f, 0.761f, 1.0f);
      }
      setting =  modes.WIREFRAME;
    }

    if (get_dim(gui.sdl2.window, &gui.window)) {
      glViewport(0, 0, gui.window.dim.w, gui.window.dim.h);
      float aspect = (float)gui.window.dim.w / (float)gui.window.dim.h;
      camera_glob->projection = mat4_perspective(70.0f, aspect, 0.1f, 1000.0f);
    }

    update_lighting(&lighting, &sun);
    if(modes.WIREFRAME){
      lighting.ambient = 1.0f;
    }


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    rotate_object(render_array.object[1]);
    render_array.object[4]->pos = player_glob->pos;
    render_array.object[4]->remodel = 1;
    for (int i=0; i<render_array.nobjects; i++) {
      if (i != 4 || player_glob->perspective) {
        draw_object(render_array.object[i], camera_glob, &lighting);
      }
    }


    gl_error_check();
    SDL_GL_SwapWindow(gui.sdl2.window);
  }


  SDL_DestroyWindow(gui.sdl2.window);
  SDL_Quit();
}