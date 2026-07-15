#ifndef EDITOR_H
#define EDITOR_H

#include <gio.h>
#include <SDL2/SDL.h>
#include <sys/stat.h>

#define event_rate 10000 //100x a sec

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef struct boolean_struct{
    uint8_t RUNNING: 1;
    uint8_t DYNAMIC: 1;
}boolean_t;
extern boolean_t modes;

typedef struct debug_selector{
    uint8_t chunk_borders:1;
    uint8_t cell_borders:1;
}debug_t;
extern debug_t debug;

typedef struct mouse_data{
    uint32_t x;
    uint32_t y;
}mouse_t;

typedef struct sdl2_data{
    SDL_Window *window;
    SDL_Renderer *renderer;
}sdl2_t;
typedef struct window_data{
    uint32_t w;
    uint32_t h;
}window_t;

typedef struct gui_engine_data{
    sdl2_t sdl2;
    window_t dim;
    mouse_t mouse;
}gui_engine_t;



extern world_t world;
extern pthread_mutex_t origin_lock;


extern int origin_chunk_x;
extern int origin_chunk_y;

extern uint32_t selected_terrain;
extern uint8_t painting;

void boot_menu(void);

int get_dim(SDL_Window *window, window_t *dim);
int get_mouse(mouse_t *mouse);
SDL_Window *init_editor_window(window_t dimensions);
SDL_Renderer *init_editor_renderer(SDL_Window *window);
sdl2_t sdl2init(window_t dimensions);
gui_engine_t gui_engine_init(void);



void save_chunk(world_tile_t chunk);
void create_dynamic(int x, int y);
void delete_dynamic(int x, int y);
void dynamic_loader(void);
void create_chunk(char *path);
void load_chunk_single(char *path);
void init_cell(cell_t *cell);
void init_chunk(chunk_t *chunk);
void chunk_editor(int x, int y);
void chunk_creator(int x, int y);

void draw_chunk(gui_engine_t *gui,u8 world_r, u8 world_c, i32 ch_origin_y, i32 ch_origin_x);
void set_color(SDL_Renderer *renderer, hexcode_u color);
void clear_screen(SDL_Renderer *renderer);
void draw_rect(SDL_Renderer *renderer, SDL_Rect *rect, hexcode_u color);
void init_events(gui_engine_t *gui);

void load_world(void);
void paint(gui_engine_t *gui);
void *editor_event_handler(void *);
void start_editor(void);


#endif
