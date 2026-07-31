// clang-format off
#include "render.h"


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

  glUseProgram(object->shader.program);
  glBindVertexArray(object->mesh->vao);

  glUniformMatrix4fv(object->shader.projection, 1, GL_FALSE, camera->projection.i);

  glUniformMatrix4fv(object->shader.model, 1, GL_FALSE, object->model.i);

  glUniformMatrix4fv(object->shader.view, 1, GL_FALSE, camera->view.i);

  glUniform3fv(glGetUniformLocation(object->shader.program, "light_ray_direction"), 1, &lighting->direction.x);

  glUniform1f(glGetUniformLocation(object->shader.program, "ambient_light"), lighting->ambient);

  glUniform1i(glGetUniformLocation(object->shader.program, "palette"), 0);

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
void update_sun_direction(sun_cycle_t *sun_c, sun_t *sun){
    if (!sun_c->visible) {
        sun->direction = vec3_fill(0);
        return;
    }

    float radians = sun_c->angle * (M_PI / 180.0f);

    sun->direction = (vec3){
        cosf(radians),
        -sinf(radians),
         0.0f
    };
}

  void update_lighting(lighting_t *lighting, sun_t *sun){
    update_camera(camera_glob);
    Uint32 now = SDL_GetTicks();

    float elapsed = (now - sun->cycle.start_time) / 270.0f;

    if(sun->cycle.visible){

      sun->cycle.angle = (elapsed / 300.0f) * 270.0f;
      if(sun->cycle.angle<90 && sun->cycle.angle>0){
        lighting->ambient = sun->cycle.angle / 100.0f + 0.1f;
      }else if(sun->cycle.angle>90 && sun->cycle.angle<180){
        lighting->ambient = (180.0f - sun->cycle.angle) / 100.0f + 0.1f;
      }
      if(lighting->ambient > 0.3) lighting->ambient = 0.3;
      

      if(elapsed >= 300.0f){
        sun->cycle.start_time = now;
        sun->cycle.visible = 0;
        sun->cycle.angle = 270.0f;
      }

    }else{
      lighting->ambient = 0.1f;
      // hidden for 30 seconds
      if(elapsed >= 300.0f){
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

  float height_func(float x, float z){
      return 
        sinf(x * 0.08f) * 3.0f +
        cosf(z * 0.08f) * 3.0f;

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

          // (vertices + r*$chunk_verts + c)->uv = (vec2){
          //   (float)c / $chunk_quads,
          //   (float)r / $chunk_quads
          // };
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
  

  GLint gl_success_code;
  hexcode_u color;


  //terrain gen:
  logical_chunk_t terrain = generate_terrain();
  glob_terrain = &terrain;
  mesh_generate_normals(terrain.object.mesh);
  load_mesh(terrain.object.mesh);
  terrain.object.shader = init_shader("./src/render/basic.vert",
                                "./src/render/basic.frag");
  terrain.object.remodel = 1;
  terrain.object.pos    = (vec3){ 0 , 0 , 0};
  terrain.object.rot    = (vec3){ 0 , 0 , 0 };
  terrain.object.scale  = (vec3){ 1 , 1 , 1 };



  vertex_t *vertices = NULL;
  tri_t *triangles = NULL;


  // objects defined in header file
  demo_tri_;
  mesh_t demo_tri_mesh = new_mesh(3, vertices, 1, triangles);
  mesh_generate_normals(&demo_tri_mesh);
  load_mesh(&demo_tri_mesh);

  render_object_t demo_tri;
  demo_tri.mesh = &demo_tri_mesh;
  demo_tri.shader = init_shader("./src/render/basic.vert",
                                "./src/render/basic.frag");
  demo_tri.pos    = (vec3){ 0 , (terrain_height((vec3){0,0,-10}) + 1.0f) ,-10};
  demo_tri.rot    = (vec3){ 0 , 0 , 0 };
  demo_tri.scale  = (vec3){ 1 , 1 , 1 };
  demo_tri.remodel  = 1;


  render_object_t demo_tri2;
  demo_tri2.mesh = &demo_tri_mesh;
  demo_tri2.shader = init_shader("./src/render/basic.vert",
                                "./src/render/basic.frag");
  demo_tri2.pos    = (vec3){ 5 ,(terrain_height((vec3){5,0,8}) + 3.0f) ,-8};
  demo_tri2.rot    = (vec3){ 1.0f , 1.0f , 1.0f };
  demo_tri2.scale  = (vec3){ 1 , 1 , 1 };
  demo_tri2.remodel  = 1;



  tower_
  mesh_t tower_mesh = new_mesh(32, vertices, 48, triangles);
  mesh_generate_normals(&tower_mesh);
  load_mesh(&tower_mesh);

  render_object_t tower;
  tower.mesh = &tower_mesh;
  tower.shader = init_shader("./src/render/basic.vert",
                              "./src/render/basic.frag");
  tower.pos = (vec3){-20,(terrain_height((vec3){-20,0,-20}) + 0.0f),-20};
  tower.rot    = (vec3){ 0 , 0 , 0 };
  tower.scale  = (vec3){ 1 , 1 , 1 };
  tower.remodel = 1;



  render_object_t tower2;
  tower2.mesh = &tower_mesh;
  tower2.shader = init_shader("./src/render/basic.vert",
                              "./src/render/basic.frag");
  tower2.pos = (vec3){20,(terrain_height((vec3){20,0,-20}) + 0.0f),-20};
  tower2.rot    = (vec3){ 0 , 0 , 0 };
  tower2.scale  = (vec3){ .5f , .5f , .5f };
  tower2.remodel = 1;



  roof_
  mesh_t roof_mesh = new_mesh(9, vertices, 8, triangles);
  mesh_generate_normals(&roof_mesh);
  load_mesh(&roof_mesh);


  render_object_t roof;
  roof.mesh = &roof_mesh;
  roof.shader = init_shader("./src/render/basic.vert",
                             "./src/render/basic.frag");
  roof.pos = (vec3){-20,(terrain_height((vec3){-20,0,-20}) + 15.0f),-20};
  roof.rot    = (vec3){ 0 , 0 , 0 };
  roof.scale  = (vec3){ 1 , 1 , 1 };
  roof.remodel = 1;

  render_object_t roof2;
  roof2.mesh = &roof_mesh;
  roof2.shader = init_shader("./src/render/basic.vert",
                             "./src/render/basic.frag");
  roof2.pos     = (vec3){20,(terrain_height((vec3){20,0,-20}) + 7.5f),-20};
  roof2.rot    = (vec3){ 0 , 0 , 0 };
  roof2.scale  = (vec3){ .5f , .5f , .5f };
  roof2.remodel = 1;


  
  camera_glob->projection = mat4_perspective(
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

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);




  free(pixels);


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

  lighting_t lighting = {0};
  lighting.ambient = .5f;
  glUseProgram(terrain.object.shader.program);
  glBindVertexArray(terrain.object.mesh->vao);
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

  update_model(&terrain.object);



  glUniformMatrix4fv(terrain.object.shader.projection, 1, GL_FALSE, camera_glob->projection.i);

  glUniformMatrix4fv(terrain.object.shader.model, 1, GL_FALSE, terrain.object.model.i);

  glUniformMatrix4fv(terrain.object.shader.view, 1, GL_FALSE, camera_glob->view.i);

  glUniform3fv(glGetUniformLocation(terrain.object.shader.program, "light_ray_direction"), 1, &lighting.direction.x);

  glUniform1f(glGetUniformLocation(terrain.object.shader.program, "ambient_light"), lighting.ambient);

  glUniform1i(glGetUniformLocation(terrain.object.shader.program, "palette"), 0);

  //mul by 3 for # of idx per tri
  glDrawElements(GL_TRIANGLES, terrain.object.mesh->ntris * 3, GL_UNSIGNED_INT, 0);

    
    
    //draw_object(&terrain.object, camera_glob, &lighting);
    
     //draw_object(&demo_tri, camera_glob, &lighting);
     //draw_object(&demo_tri2, camera_glob, &lighting);

     //draw_object(&tower, camera_glob, &lighting);
     //draw_object(&roof, camera_glob, &lighting);

     //draw_object(&tower2, camera_glob, &lighting);
     //draw_object(&roof2, camera_glob, &lighting);

    gl_error_check();

    SDL_GL_SwapWindow(gui.sdl2.window);

  }


  SDL_DestroyWindow(gui.sdl2.window);
  SDL_Quit();
}