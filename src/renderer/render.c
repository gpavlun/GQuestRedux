// clang-format off
#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include <gcurses.h>
#include <gio.h>
#include <logging.h>

#include "editor.h"
#include "render.h"

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#define def typedef struct

char glob_error[64] = "no error";

boolean_t modes;
debug_t debug;

world_t world;
pthread_mutex_t origin_lock;

int origin_chunk_x;
int origin_chunk_y;

uint32_t selected_terrain;
uint8_t painting;

def shared_data { 
  float dist; 
}data_t;
data_t sh;

def vector_3val_data {
  float x, y, z;
}
vec3_t;
def player_data {
  vec3_t pos;
  vec3_t vel;
  vec3_t acc;
  vec3_t applied_force;
  float max_force;
  float mass;
}player_t;
player_t player;


def object_data {
  vec3_t pos;
  vec3_t rel;
  dimen_t dim;
}object_t;
object_t object_glob;

def vertice_data{ 
  float x, y, z; 
}vert_t;

def edge_data{ 
  u32 A, B; 
}edge_t;

def triangle_data{ 
  u16 a, b, c; 
}triangle_t;

def {
  u8 wire_frame: 1;
  u8 triangles: 1;
  u8 vertices: 1;
}mesh_opts;

def mesh_data {
  u16 nedges;
  edge_t *edges;
  u16 nverts;
  vert_t *verts;
  u16 ntrangs;
  triangle_t *trangs;
  hexcode_u color;
  mesh_opts opts;
}mesh_t;

def inputs {
  u8 w : 1;
  u8 a : 1;
  u8 s : 1;
  u8 d : 1;
  u8 space : 1;
}inputs_t;




#define DRAG_CONSTANT 200
#define planetary_mass 10000000000000000000000000
#define planitary_diam 12756000
#define gravitational_const 0.000000000067408
#define cheat_grav 9.8

#define sensitivity 0.5

float theta;
float phi;

void game_logic(float dt) {

  vec3_t drag_force;
  drag_force.x = -(DRAG_CONSTANT * player.vel.x);
  drag_force.z = -(DRAG_CONSTANT * player.vel.z);

  double gravity_force;
  gravity_force = -cheat_grav * player.mass;

  vec3_t net_force;
  net_force.x = player.applied_force.x + drag_force.x;
  net_force.z = player.applied_force.z + drag_force.z;
  net_force.y = player.applied_force.y + gravity_force;

  player.acc.x = net_force.x / player.mass;
  player.acc.z = net_force.z / player.mass;
  player.acc.y = net_force.y / player.mass;

  player.vel.x += player.acc.x * dt;
  player.vel.z += player.acc.z * dt;
  player.vel.y += player.acc.y * dt;

  player.pos.x += player.vel.x * dt;
  player.pos.z += player.vel.z * dt;

  player.pos.y += player.vel.y * dt;
  if (player.pos.y < 2) {
    player.pos.y = 2;

    if (player.vel.y < 0)  player.vel.y = 0;
  }
}

void multipress(inputs_t key){

  player.applied_force.x = player.applied_force.y = player.applied_force.z = 0;
  float input_x = 0, input_z = 0;

  if(key.w){
    input_z += 1;
  }

  if(key.s){
    input_z -= 1;
  }

  if(key.a){
    input_x -= 1;
  }

  if(key.d){
    input_x += 1;
  }

  if(key.space) player.applied_force.y = player.max_force;

  float world_x;
  float world_z;

  world_x = input_x * cos(theta) + input_z * sin(theta);
  world_z = -input_x * sin(theta) + input_z * cos(theta);

  player.applied_force.x = world_x * player.max_force;
  player.applied_force.z = world_z * player.max_force;
}


void *editor_event_handler(void *args){
  gui_engine_t *gui = (gui_engine_t *)args;
  SDL_Event event;
  const uint8_t *keyscan = SDL_GetKeyboardState(NULL);
  inputs_t inputs;
  u64 last_movement = SDL_GetPerformanceCounter();

  u64 current_movement;
  double dt;

  player.mass = 80;
  player.max_force = 1200;

  u8 mousemode = 0;

  while(modes.RUNNING){
    current_movement = SDL_GetPerformanceCounter();
    dt = (double)(current_movement - last_movement) /
         (double)SDL_GetPerformanceFrequency();

    while(SDL_PollEvent(&event)){
      switch(event.type){
        case(SDL_QUIT):{
          
          modes.RUNNING = 0;

        $}case(SDL_KEYUP):{
          switch(event.key.keysym.sym){
            case(SDLK_ESCAPE):{
              modes.RUNNING = 0;
            $}default:{$}
          }
        $}case(SDL_MOUSEMOTION):{
          if(mousemode){
            theta += event.motion.xrel * 0.01f;
            phi -= event.motion.yrel * 0.01f;

            if(phi>1.5) phi = 1.5;
            else if(phi<-1.5) phi = -1.5;
          }
        $}case(SDL_MOUSEBUTTONUP):{
          if(mousemode) SDL_SetRelativeMouseMode(SDL_FALSE);
          else SDL_SetRelativeMouseMode(SDL_TRUE);

          mousemode = !mousemode;
        $}default:{$}
      }

      inputs.w = keyscan[SDL_SCANCODE_W];
      inputs.a = keyscan[SDL_SCANCODE_A];
      inputs.s = keyscan[SDL_SCANCODE_S];
      inputs.d = keyscan[SDL_SCANCODE_D];
      inputs.space = keyscan[SDL_SCANCODE_SPACE];

      multipress(inputs);
    }

    game_logic(dt);
    last_movement = current_movement;
    usleep(event_rate);
  }

  pthread_exit(0);
}


def point_data {
  int x, y;
}
point_t;

def vis_point_data {
  int x, y;
  u8 visible;
}
vpoint_t;

int max(int a, int b, int c) {
  if (a>b && a>c)
    return a;
  if (b>a && b>c)
    return b;
  return c;
}

int min(int a, int b, int c) {
  if (a<b && a<c)
    return a;
  if (b<a && b<c)
    return b;
  return c;
}



float edge(point_t a, point_t b, float x, float y) {
  return (x - a.x) * (b.y - a.y) - (y - a.y) * (b.x - a.x);
}

vec3_t find_intersect(vec3_t a, vec3_t b) {
  float t = (0.1f - a.z) / (b.z - a.z);

  vec3_t out;

  out.x = a.x + (b.x - a.x) * t;
  out.y = a.y + (b.y - a.y) * t;
  out.z = 0.1f;

  return out;
}

def tremp_t_dat {
  vec3_t a, b, c;
  u8 valid;
}
tri_t;
void clip_one(vec3_t a, vec3_t b, vec3_t c, tri_t *trangs) {
  // clips point 'a' off of a triangle
  vec3_t v1, v2;
  v1 = find_intersect(a, b);
  v2 = find_intersect(a, c);
  trangs[0].a = v1;
  trangs[0].b = c;
  trangs[0].c = v2;
  trangs[0].valid = 1;

  trangs[1].a = v1;
  trangs[1].b = b;
  trangs[1].c = c;
  trangs[1].valid = 1;
}

void clip_two(vec3_t a, vec3_t b, vec3_t c, tri_t *trangs){
  // clips point 'a' and 'b' off of a triangle
  vec3_t v1, v2;
  v1 = find_intersect(a, c);
  v2 = find_intersect(b, c);
  trangs[0].a = v1;
  trangs[0].b = c;
  trangs[0].c = v2;
  trangs[0].valid = 1;

  trangs[1].valid = 0;
}
void clear_buffer(framebuffer_t *fb, uint32_t color){
    for(int i=0; i<fb->width*fb->height; i++){
        fb->pixels[i] = color;
    }
}
void put_pixel(framebuffer_t *fb, int x, int y, uint32_t color){
    if(x<0 || x>=fb->width ||
       y<0 || y>=fb->height)
        return;
    fb->pixels[y * fb->width + x] = color;
}
void draw_line_buffer(framebuffer_t *fb, int x0, int y0, int x1, int y1, u32 color){
  int dx, dy, sx, sy, err, e2;

  dx = abs(x1 - x0);
  dy = abs(y1 - y0);

  if(x0 < x1) sx = 1;
  else sx = -1;

  if(y0 < y1) sy = 1;
  else sy = -1;

  err = dx - dy;

  while(1){
    put_pixel(fb, x0, y0, color);

    if(x0 == x1 && y0 == y1) break;
    
    e2 = 2 * err;
    
    if(e2 > -dy){
      err -= dy;
      x0 += sx;
    }
    if(e2 < dx){
      err += dx;
      y0 += sy;
    }
  }
}
void draw_rect_buffer(framebuffer_t *fb, SDL_Rect rect, u32 color){
  int x, y, start_x, start_y, end_x, end_y;

  start_x = rect.x;
  start_y = rect.y;
  end_x = rect.x + rect.w;
  end_y = rect.y + rect.h;

  if(start_x < 0) start_x = 0;
  if(start_y < 0) start_y = 0;
  if(end_x > fb->width) end_x = fb->width;
  if(end_y > fb->height)end_y = fb->height;

  for(y = start_y; y < end_y; y++){
    for(x = start_x; x < end_x; x++){
      put_pixel(fb, x, y, color);
    }
  }
}

void draw_mesh_triangle(gui_engine_t *gui, mesh_t mesh){
  window_t *window = &gui->window;
  vec3_t relative;
  vec3_t rotated;
  vec3_t pitched;
  float focal = 400;

  float screen_x, screen_y;
  
  vec3_t trans_vert[mesh.nverts];
  point_t points[3];

  int i, j, y, x;

  for(i=0; i<mesh.nverts; i++){
    relative.x = mesh.verts[i].x - player.pos.x;
    relative.y = mesh.verts[i].y - player.pos.y;
    relative.z = mesh.verts[i].z - player.pos.z;

    rotated.x = relative.x * cos(theta) - relative.z * sin(theta);
    rotated.y = relative.y;
    rotated.z = relative.x * sin(theta) + relative.z * cos(theta);

    pitched.x = rotated.x;
    pitched.y = rotated.y * cos(phi) - rotated.z * sin(phi);
    pitched.z = rotated.y * sin(phi) + rotated.z * cos(phi);

    trans_vert[i].x = pitched.x;
    trans_vert[i].y = pitched.y;
    trans_vert[i].z = pitched.z;
  }

  #define NEAR 0.1f

  float e1, e2, e3;
  float area;
  int inside;
  i8 inA, inB, inC;
  

  if(!mesh.ntrangs && mesh.opts.wire_frame){
    set_color(gui->sdl2.renderer, mesh.color);
    vpoint_t vpoints[mesh.nverts];
    SDL_Rect verts_vis[mesh.nverts];
    for(i=0; i<mesh.nverts; i++){
      if(trans_vert[i].z<=0.01f){
        vpoints[i].visible = 0;
        continue;
      }
      vpoints[i].visible = 1;

      screen_x = trans_vert[i].x / trans_vert[i].z;
      screen_y = trans_vert[i].y / trans_vert[i].z;
      if(mesh.opts.vertices){
        verts_vis[i].w = verts_vis[i].h = (int)(.25 * focal / trans_vert[i].z);
        verts_vis[i].x = window->cen.x + (int)(screen_x * focal) - verts_vis[i].w/2;
        verts_vis[i].y = window->cen.y - (int)(screen_y * focal) - verts_vis[i].h/2;
        draw_rect_buffer(gui->window.fb, verts_vis[i], 0xff000000 | mesh.color.code);      
      }

      vpoints[i].x = window->cen.x + (int)(screen_x * focal);
      vpoints[i].y = window->cen.y - (int)(screen_y * focal);
    }

    for(i=0; i<mesh.nedges; i++){
      if(vpoints[mesh.edges[i].A].visible && vpoints[mesh.edges[i].B].visible){
          draw_line_buffer(gui->window.fb,
                           vpoints[mesh.edges[i].A].x,
                           vpoints[mesh.edges[i].A].y,
                           vpoints[mesh.edges[i].B].x,
                           vpoints[mesh.edges[i].B].y,
                           0xff000000 | mesh.color.code);
      }
    }
    return;
  }


  tri_t trangs[2] = {0};
  for(i=0; i<mesh.ntrangs; i++){
    inA = trans_vert[mesh.trangs[i].a].z>=NEAR;
    inB = trans_vert[mesh.trangs[i].b].z>=NEAR;
    inC = trans_vert[mesh.trangs[i].c].z>=NEAR;

    inside = (int)inA + (int)inB + (int)inC;
    switch(inside){
      case 0:
        // all outside
        continue;

      case 3:
        // all inside
        trangs[0].a = trans_vert[mesh.trangs[i].a];
        trangs[0].b = trans_vert[mesh.trangs[i].b];
        trangs[0].c = trans_vert[mesh.trangs[i].c];
        trangs[0].valid = 1;
        trangs[1].valid = 0;
        goto render;

      case 2:// one outside
        if(!inA){ /* A outside */
          clip_one(trans_vert[mesh.trangs[i].a],
                   trans_vert[mesh.trangs[i].b],
                   trans_vert[mesh.trangs[i].c],
                   trangs);
        }else if(!inB){ /* B outside */
          clip_one(trans_vert[mesh.trangs[i].b],
                   trans_vert[mesh.trangs[i].c],
                   trans_vert[mesh.trangs[i].a],
                   trangs);
        }else{ /* C outside */
          clip_one(trans_vert[mesh.trangs[i].c],
                   trans_vert[mesh.trangs[i].a],
                   trans_vert[mesh.trangs[i].b],
                   trangs);
        }
        break;

      case 1:// two outside
        if(inA){ /* B and C outside */
          clip_two(trans_vert[mesh.trangs[i].b],
                   trans_vert[mesh.trangs[i].c],
                   trans_vert[mesh.trangs[i].a],
                   trangs);
        }else if(inB){ /* A and C outside */
          clip_two(trans_vert[mesh.trangs[i].c],
                   trans_vert[mesh.trangs[i].a],
                   trans_vert[mesh.trangs[i].b],
                   trangs);
        }else{ /* A and B outside */
          clip_two(trans_vert[mesh.trangs[i].a],
                   trans_vert[mesh.trangs[i].b],
                   trans_vert[mesh.trangs[i].c],
                   trangs);
        }
        break;
    }

  render:

    for(j=0; j<2; j++){
      if(trangs[j].valid){
        screen_x = trangs[j].a.x / trangs[j].a.z;
        screen_y = trangs[j].a.y / trangs[j].a.z;
        points[0].x = window->cen.x + (int)(screen_x * focal);
        points[0].y = window->cen.y - (int)(screen_y * focal);


        screen_x = trangs[j].b.x / trangs[j].b.z;
        screen_y = trangs[j].b.y / trangs[j].b.z;
        points[1].x = window->cen.x + (int)(screen_x * focal);
        points[1].y = window->cen.y - (int)(screen_y * focal);

        screen_x = trangs[j].c.x / trangs[j].c.z;
        screen_y = trangs[j].c.y / trangs[j].c.z;
        points[2].x = window->cen.x + (int)(screen_x * focal);
        points[2].y = window->cen.y - (int)(screen_y * focal);

        int min_x = min(points[0].x, points[1].x, points[2].x);
        int max_x = max(points[0].x, points[1].x, points[2].x);
        int min_y = min(points[0].y, points[1].y, points[2].y);
        int max_y = max(points[0].y, points[1].y, points[2].y);

        if(min_x<0) min_x = 0;
        if(min_y<0) min_y = 0;
        if(max_x>=window->dim.w) max_x = window->dim.w - 1;
        if(max_y>=window->dim.h) max_y = window->dim.h - 1;
        
        set_color(gui->sdl2.renderer, mesh.color);
        if(mesh.opts.wire_frame){
          draw_line_buffer(gui->window.fb,
                           points[1].x,
                           points[1].y,
                           points[0].x,
                           points[0].y,
                          0xff000000 | mesh.color.code);
          draw_line_buffer(gui->window.fb,
                           points[2].x,
                           points[2].y,
                           points[1].x,
                           points[1].y,
                           0xff000000 | mesh.color.code);
          draw_line_buffer(gui->window.fb,
                           points[2].x,
                           points[2].y,
                           points[0].x,
                           points[0].y,
                           0xff000000 | mesh.color.code);
        }

        if(mesh.opts.triangles){

          area = edge(points[0], points[1], points[2].x, points[2].y);

          if(area<0){
              point_t tmp = points[1];
              points[1] = points[2];
              points[2] = tmp;

              area = -area;
          }

          if(area == 0) continue;

          for(y = min_y; y<=max_y; y++){
            for(x = min_x; x<=max_x; x++){
              e1 = edge(points[0], points[1], x, y);
              e2 = edge(points[1], points[2], x, y);
              e3 = edge(points[2], points[0], x, y);

              if((e1>=0 && e2>=0 && e3>=0) ||
                (e1<=0 && e2<=0 && e3<=0)){
                put_pixel(gui->window.fb, x, y, 0xff000000 | mesh.color.code);
              }
            }
          }
        }
      }
    }
  }
}

void start_render(void) {

  modes.RUNNING = 1;

  gui_engine_t gui = gui_engine_init();
  init_events(&gui);
  window_t *window = &gui.window;
  

  // the concept here is to have a buffer to store
  // the pixels in before writing them to the screen
  framebuffer_t fb;
  fb.width = window->dim.w;
  fb.height = window->dim.h;
  fb.pixels = malloc(fb.width * fb.height * sizeof(u32));
  gui.window.fb = &fb;

  SDL_Texture *screen_texture;
  screen_texture = SDL_CreateTexture(gui.sdl2.renderer,
                                     SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     fb.width,
                                     fb.height);

  hexcode_u color;
  render_frame_t render;


  player.pos.x = 0;
  player.pos.y = 2;
  player.pos.z = 0;

  // mesh construction defined in header
  cubiod_
  ground_






  while (modes.RUNNING) {

    if(get_dim(gui.sdl2.window, window)){
      fb.width = window->dim.w;
      fb.height = window->dim.h;
      free(fb.pixels);
      fb.pixels = malloc(fb.width * fb.height * sizeof(u32));
      SDL_DestroyTexture(screen_texture);
      screen_texture = SDL_CreateTexture(gui.sdl2.renderer,
                                        SDL_PIXELFORMAT_ARGB8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        fb.width,
                                        fb.height);
    }

    clear_buffer(&fb, 0xff7cafc2);
    //clear_screen_wrap(gui.sdl2.renderer);

    draw_mesh_triangle(&gui, ground);
    draw_mesh_triangle(&gui, cuboid);

    SDL_UpdateTexture(screen_texture,
                      NULL,
                      fb.pixels,
                      fb.width * sizeof(u32));
    SDL_RenderCopy(gui.sdl2.renderer,
                   screen_texture,
                   NULL,
                   NULL);
    SDL_RenderPresent(gui.sdl2.renderer);
    usleep(event_rate);
  }

  SDL_DestroyRenderer(gui.sdl2.renderer);
  SDL_DestroyWindow(gui.sdl2.window);
  SDL_Quit();
  return;
}

void *tui(void *temp) {

  int *running = (int *)temp;

  term_w_t terminal;
  init_tui(&terminal);
  terminal.io_block(0);
  terminal.clear();
  terminal.present();

  char input = EOF;
  char message[64];

  strcpy(message, "startup complete");

  int selected = 0;

  char source[128];
  str_t str;

  while (*running) {

    terminal.clear();

    str.c = 1;
    str.source = source;

    str.r = 1;
    sprintf(source, "player");
    terminal.horz_strdisp(str);

    str.r = 2;
    sprintf(source, "    [s] : (%.2f m, %.2fm, %.2fm)", player.pos.x,
            player.pos.y, player.pos.z);
    terminal.horz_strdisp(str);

    str.r = 3;
    sprintf(source, "    [v] : (%.2f ms, %.2fms, %.2fms)", player.vel.x,
            player.vel.y, player.vel.z);
    terminal.horz_strdisp(str);

    str.r = 4;
    sprintf(source, "    [a] : (%.2f ms^2, %.2fms^2, %.2fms^2)", player.acc.x,
            player.acc.y, player.acc.z);
    terminal.horz_strdisp(str);

    str.r = 5;
    sprintf(source, "    applied force : (%.2f N, %.2fN, %.2fN)",
            player.applied_force.x, player.applied_force.y,
            player.applied_force.z);
    terminal.horz_strdisp(str);

    str.r = 6;
    sprintf(source, "theta %.2f :: phi %.2f", theta, phi);
    terminal.horz_strdisp(str);

    str.r = 10;
    sprintf(source, "error message: %s", glob_error);
    terminal.horz_strdisp(str);

    terminal.present();

    usleep(10000);
  }

  //terminal.clear();
  //terminal.present();
  terminal.io_block(1);
  terminal.cursor(1);
  move_cursor(&terminal, terminal.nrows, 0);
  pthread_exit(0);
}

int main(void) {

  int running = 1;
  pthread_t tui_thread;
  pthread_create(&tui_thread, NULL, tui, &running);

  start_render();
  // pthread_t logic_thread;
  // pthread_create(&logic_thread, NULL, game_logic, &running);

  running = 0;
  usleep(100000);
  pthread_cancel(tui_thread);
  pthread_join(tui_thread, NULL);
  return 0;
}
