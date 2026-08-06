#ifndef EDITOR_H
#define EDITOR_H

#include <gio.h>
#include <SDL2/SDL.h>

#include "../main/typing.h"

#define event_rate 10000 //100x a sec




def {
    u8 chunk_borders:1;
    u8 cell_borders:1;
}debug_t;
extern debug_t debug;

def {
    i32 x;
    i32 y;
}mouse_t;

def {
    i32 x;
    i32 y;
}coord_t;

def {
    i32 w;
    i32 h;
}dimen_t;

def {
    dimen_t dim;
    coord_t cen;
    SDL_Window *interface;
}window_t;

def {
    window_t window;
    mouse_t mouse;
}gui_data_t;




int get_dim(window_t *win);
int get_mouse(mouse_t *mouse);
SDL_Window *init_sdl_window(dimen_t dim);
void init_sdl_engine(gui_data_t *gui);



void clear_screen_wrap(SDL_Renderer *renderer);
void draw_rect_wrap(SDL_Renderer *renderer, SDL_Rect *rect, hexcode_u color);

void *editor_event_handler(void *);

#endif
