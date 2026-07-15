#include <SDL2/SDL.h>
#include <pthread.h>

#include <gio.h>
#include <logging.h>
#include "editor.h"

int get_dim(SDL_Window *window, window_t *dim){
    int new_width;
    int new_height;
    int result = 0;
    SDL_GetWindowSize(window, &new_width, &new_height);

    dim->w = new_width;
    dim->h = new_height;
    return 0;  
}
int get_mouse(mouse_t *mouse){
    SDL_GetMouseState(&mouse->x, &mouse->y);
    return 0;  
}

SDL_Window *init_editor_window(window_t dimensions){

    SDL_Window *window = 
        SDL_CreateWindow(
            "Interface",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            dimensions.w,
            dimensions.h,
            SDL_WINDOW_RESIZABLE |
            SDL_WINDOW_SHOWN);

    if(window)  
            logging.info("Window initialized");
    else    logging.error(NO_RETURN, "Window failed");

    return window;
}

SDL_Renderer *init_editor_renderer(SDL_Window *window){
    SDL_Renderer* renderer =
        SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC);

    if(renderer)
            logging.info("Renderer initialized");
    else    logging.error(NO_RETURN, "Renderer failed");

    return renderer;  
}

sdl2_t sdl2init(window_t dimensions){
    
    SDL_Window *window = init_editor_window(dimensions);

    SDL_Renderer* renderer = init_editor_renderer(window);

    sdl2_t sdl2;
    sdl2.window = window;
    sdl2.renderer = renderer;

    return sdl2;
}

gui_engine_t gui_engine_init(void){
    if(SDL_Init(SDL_INIT_VIDEO))            
            logging.error(NO_RETURN, "SDL2 failed to init");      
    else    logging.info("SDL2 initialized");
    
    gui_engine_t gui;
    get_mouse(&gui.mouse);
    gui.dim.w = CHUNK_SIZE*CELL_RESOLUTION + 200;
    gui.dim.h = CHUNK_SIZE*CELL_RESOLUTION + 200;    
    gui.sdl2 = sdl2init(gui.dim);
    
    return gui;
}

void init_events(gui_engine_t *gui){
    pthread_t event_thread;
    if(pthread_create(&event_thread, NULL, editor_event_handler, gui))
            logging.error(NO_RETURN, "Event thread failed");        
    else    logging.info("Event thread initialized");

}
