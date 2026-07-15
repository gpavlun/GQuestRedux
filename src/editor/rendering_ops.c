#include <SDL2/SDL.h>


#include <gio.h>
#include <logging.h>

#include "editor.h"


void set_color(SDL_Renderer *renderer, hexcode_u color){
    SDL_SetRenderDrawColor(renderer, color.rgba.red, color.rgba.green, color.rgba.blue, color.rgba.opacity);
}
void clear_screen(SDL_Renderer *renderer){
    hexcode_u color;
    color.code = 0x1E1E1E;
    set_color(renderer, color);
    SDL_RenderClear(renderer);    
}
void draw_rect(SDL_Renderer *renderer, SDL_Rect *rect, hexcode_u color){
    set_color(renderer, color);
    SDL_RenderDrawRect(renderer, rect);
}


void draw_chunk(gui_engine_t *gui,u8 world_r, u8 world_c, i32 ch_origin_y, i32 ch_origin_x){
    i32 ch_width = CHUNK_SIZE*CELL_RESOLUTION;
    
    u8 row, col;
    hexcode_u color;

    cell_t *cell;
    SDL_Rect cell_box;
    cell_box.w = cell_box.h = CELL_RESOLUTION;

    for(row=0; row<CHUNK_SIZE; row++){
        cell_box.y = ch_origin_y + world_r*ch_width + row*CELL_RESOLUTION;
        for(col=0; col<CHUNK_SIZE; col++){
            cell_box.x = ch_origin_x + world_c*ch_width + col*CELL_RESOLUTION;
            cell = &world.array[world_r][world_c].chunk->cell_array[row][col];
            
            switch(cell->terrain.type){
                case(none_tt):{
                    color.code = 0x000000;
                $}case(grass_tt):{
                    color.code = 0x44a83a;
                $}case(stone_tt):{
                    color.code = 0x4d4a4a;
                $}case(water_tt):{
                    color.code = 0x1055ed;
                $}default:$
            }
            
            set_color(gui->sdl2.renderer, color);
            SDL_RenderFillRect(gui->sdl2.renderer, &cell_box);
            if(debug.cell_borders){
                color.code = 0x0000FF;
                set_color(gui->sdl2.renderer, color);
                SDL_RenderDrawRect(gui->sdl2.renderer, &cell_box);
            }   
        }
    }
}