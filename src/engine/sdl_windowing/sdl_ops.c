#include <SDL2/SDL.h>

#include <gio.h>
#include <logging.h>
#include <glad/glad.h>

#include "sdl_ops.h"

int get_dim(window_t *win){
    int new_width;
    int new_height;

    SDL_GetWindowSize(win->interface, &new_width, &new_height);
    if(new_width !=  win->dim.w || new_height !=  win->dim.h){
        win->dim.w = new_width;
        win->dim.h = new_height;

        win->cen.x = new_width/2;
        win->cen.y = new_height/2;
        return 1;
    }
    return 0;  
}

int get_mouse(mouse_t *mouse){
  SDL_GetMouseState(&mouse->x, &mouse->y);
  return 0;
}

SDL_Window *init_sdl_window(dimen_t dim){

    SDL_Window *window = 
        SDL_CreateWindow(
            "Interface",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            dim.w,
            dim.h,
            SDL_WINDOW_RESIZABLE |
            SDL_WINDOW_OPENGL);

    if(!window)
      logging.error(NO_RETURN, "Window failed");

    return window;
}

void init_sdl_engine(gui_data_t *gui){
  if(SDL_Init(SDL_INIT_VIDEO))
          logging.error(NO_RETURN, "SDL2 failed to init");

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE);

  get_mouse(&gui->mouse);
  gui->window.cen.x = gui->window.dim.w / 2;
  gui->window.cen.y = gui->window.dim.h / 2;
  gui->window.interface = init_sdl_window(gui->window.dim);
}
