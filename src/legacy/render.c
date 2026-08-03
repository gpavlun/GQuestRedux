// clang-format off

#include <SDL2/SDL.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

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

    if(x0 == x1 && y0 == y1) return;
    e2 = err<<1;
    
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
void draw_vert_buffer(framebuffer_t *fb, point_t points[3], u32 color){
  int x, y, start_x, start_y, end_x, end_y;
  for(int i=0;i<3;i++){
    start_x = points[i].x;
    start_y = points[i].y;
    end_x = start_x + 5;
    end_y = start_y + 5;

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

  hexcode_u selected_color;

  int i, j, y, x;

  for(i=0; i<mesh.nverts; i++){
    relative.x = mesh.verts[i].x - player_glob.pos.x;
    relative.y = mesh.verts[i].y - player_glob.pos.y;
    relative.z = mesh.verts[i].z - player_glob.pos.z;

    rotated.x = relative.x * cosf(player_glob.theta) - relative.z * sinf(player_glob.theta);
    rotated.y = relative.y;
    rotated.z = relative.x * sinf(player_glob.theta) + relative.z * cosf(player_glob.theta);

    pitched.x = rotated.x;
    pitched.y = rotated.y * cosf(player_glob.phi) - rotated.z * sinf(player_glob.phi);
    pitched.z = rotated.y * sinf(player_glob.phi) + rotated.z * cosf(player_glob.phi);

    trans_vert[i].x = pitched.x;
    trans_vert[i].y = pitched.y;
    trans_vert[i].z = pitched.z;  
  }

  #define NEAR 0.1f

  float e1, e2, e3;
  float area;
  int inside;
  i8 inA, inB, inC;
  

  if(mesh.opts.wire_frame || mesh.opts.vertices){
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
        draw_rect_buffer(gui->window.fb, verts_vis[i], 0xffff0000);      
      }

      vpoints[i].x = window->cen.x + (int)(screen_x * focal);
      vpoints[i].y = window->cen.y - (int)(screen_y * focal);
    }
    if(!mesh.ntrangs && mesh.opts.wire_frame){
      for(i=0; i<mesh.nedges; i++){
            if(vpoints[mesh.edges[i].A].visible && vpoints[mesh.edges[i].B].visible){
                draw_line_buffer(gui->window.fb,
                                vpoints[mesh.edges[i].A].x,
                                vpoints[mesh.edges[i].A].y,
                                vpoints[mesh.edges[i].B].x,
                                vpoints[mesh.edges[i].B].y,
                                0xffff0000);
        }
      }
      return;
    }
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
      
        if(mesh.opts.wire_frame){
          draw_line_buffer(gui->window.fb,
                           points[1].x,
                           points[1].y,
                           points[0].x,
                           points[0].y,
                           0xffff0000);
          draw_line_buffer(gui->window.fb,
                           points[2].x,
                           points[2].y,
                           points[1].x,
                           points[1].y,
                           0xffff0000);
          draw_line_buffer(gui->window.fb,
                           points[2].x,
                           points[2].y,
                           points[0].x,
                           points[0].y,
                           0xffff0000);
        }

        if(mesh.opts.triangles){
          area = edge(points[0], points[1], points[2].x, points[2].y);
          //if(selected_color.code != mesh.trangs[i].color.code){
          //  selected_color = mesh.trangs[i].color.code;
          //}
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
                put_pixel(gui->window.fb, x, y, 0xff000000 | mesh.trangs[i].color.code);
              }
            }
          }
        }
      }
    }
  }
}

void start_render(void) {
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


  player_glob.pos.x = 0;
  player_glob.pos.y = 2;
  player_glob.pos.z = 0;

  // mesh construction defined in header
  cubiod_
  ground_
  tower_





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

    draw_mesh_triangle(&gui, ground);
    draw_mesh_triangle(&gui, cuboid);
    draw_mesh_triangle(&gui, tower);
    
    draw_line_buffer(&fb,fb.width/2-5,fb.height/2,fb.width/2+5 ,fb.height/2,0xFF00FF00);
    draw_line_buffer(&fb,fb.width/2,fb.height/2-5,fb.width/2 ,fb.height/2+5,0xFF00FF00);
    
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
}