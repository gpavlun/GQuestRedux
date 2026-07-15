#include <SDL2/SDL.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>

#include <gio.h>
#include <logging.h>

#include "editor.h"

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;





boolean_t modes;
debug_t debug;

world_t world;
pthread_mutex_t origin_lock;


int origin_chunk_x;
int origin_chunk_y;

uint32_t selected_terrain;
uint8_t painting;





void load_world(void){

    pthread_mutex_lock(&origin_lock);

    char path[64];
    FILE *chunk_file;

    int temp_chunk_x = origin_chunk_x - RENDER_DISTANCE/2 - (!(RENDER_DISTANCE%2));
    int temp_chunk_y = origin_chunk_y - RENDER_DISTANCE/2 - (!(RENDER_DISTANCE%2));

    int chunk_x = temp_chunk_x;
    int chunk_y = temp_chunk_y;
    int world_x,world_y;

    for(world_y=0;world_y<RENDER_DISTANCE;world_y++) for(world_x=0;world_x<RENDER_DISTANCE;world_x++){
        if(world.array[world_y][world_x].defined){ 
            save_chunk(world.array[world_y][world_x]);
            free(world.array[world_y][world_x].chunk);
            world.array[world_y][world_x].chunk = NULL;
            world.array[world_y][world_x].defined=0;

        }

    }

    for(world_y=0;world_y<RENDER_DISTANCE;world_y++){
        chunk_y = temp_chunk_y + world_y;
        for(world_x=0;world_x<RENDER_DISTANCE;world_x++){
            chunk_x = temp_chunk_x + world_x;

            sprintf(path,"%s/_%d_%d_.ck",PATH_HEADER, chunk_x, chunk_y);

            chunk_file = fopen(path, "rb");

            if(chunk_file){
                world.array[world_y][world_x].chunk = malloc(sizeof(chunk_t));
                world.array[world_y][world_x].x_pos = chunk_x;
                world.array[world_y][world_x].y_pos = chunk_y;
                if(fread(world.array[world_y][world_x].chunk, sizeof(chunk_t), 1, chunk_file)!=1) 
                    logging.error(500,"Couldn't read data!");
                fclose(chunk_file);
                world.array[world_y][world_x].defined=1;
            }
        }        
    }

    pthread_mutex_unlock(&origin_lock);
}



void paint(gui_engine_t *gui){
    int cell_x = ( gui->mouse.x - (  gui->dim.w/2 - CHUNK_SIZE*CELL_RESOLUTION / 2 ) ) / CELL_RESOLUTION;
    int cell_y = ( gui->mouse.y - (  gui->dim.h/2 - CHUNK_SIZE*CELL_RESOLUTION / 2 ) ) / CELL_RESOLUTION;
    
    if(world.array[RENDER_DISTANCE/2][RENDER_DISTANCE/2].defined){
        if(cell_x>=0&&cell_y>=0&&cell_x<CHUNK_SIZE&&cell_y<CHUNK_SIZE){
            world.array[RENDER_DISTANCE/2][RENDER_DISTANCE/2].chunk->cell_array[cell_y][cell_x].terrain.type = selected_terrain;
        }
    }
}



void *editor_event_handler(void *args){
    gui_engine_t *gui = (gui_engine_t *)args;
    SDL_Event event;
    const uint8_t *keyscan = SDL_GetKeyboardState(NULL);
    while(modes.RUNNING){

        while(SDL_PollEvent(&event)){
        
            switch(event.type){
                case(SDL_QUIT):{
                
                    modes.RUNNING = 0;
                
                $}case(SDL_KEYUP):{

                    switch(event.key.keysym.sym){
                        case(SDLK_ESCAPE):{ // window ctrls
                            modes.RUNNING = 0;
                        $}case(SDLK_o):{ // dev
                            debug.chunk_borders = !debug.chunk_borders;
                            if(debug.chunk_borders) logging.info("Chunk borders enabled");
                            else logging.info("Chunk borders disabled");
                        $}case(SDLK_p):{
                            debug.cell_borders = !debug.cell_borders;
                            if(debug.cell_borders) logging.info("Cell borders enabled");
                            else logging.info("Cell borders disabled");
                        $}case(SDLK_0):{ // brush ctrls
                            selected_terrain = none_tt;
                            logging.data("Selected terrain: ",selected_terrain);
                        $}case(SDLK_1):{
                            selected_terrain = grass_tt;
                            logging.data("Selected terrain: ",selected_terrain);
                        $}case(SDLK_2):{
                            selected_terrain = stone_tt;
                            logging.data("Selected terrain: ",selected_terrain);
                        $}case(SDLK_3):{
                            selected_terrain = water_tt;
                            logging.data("Selected terrain: ",selected_terrain);
                        $}default:{
                            if(modes.DYNAMIC){
                                switch(event.key.keysym.sym){
                                    case(SDLK_UP):{  
                                        origin_chunk_y--;
                                        load_world();
                                    $}case(SDLK_DOWN):{
                                        origin_chunk_y++;
                                        load_world();
                                    $}case(SDLK_LEFT):{
                                        origin_chunk_x--;                                    
                                        load_world();
                                    $}case(SDLK_RIGHT):{
                                        origin_chunk_x++;                                    
                                        load_world();
                                    $}case(SDLK_RETURN):{
                                        if(!world.array[RENDER_DISTANCE/2][RENDER_DISTANCE/2].defined){
                                            create_dynamic(origin_chunk_x,origin_chunk_y);
                                            load_world();
                                        }                                     
                                    $}case(SDLK_DELETE):{
                                        if(world.array[RENDER_DISTANCE/2][RENDER_DISTANCE/2].defined){
                                            delete_dynamic(origin_chunk_x,origin_chunk_y);
                                            load_world();
                                        }
                                    $}default:$
                                }
                            }
                        $}
                    }


                $}case(SDL_MOUSEBUTTONDOWN):{
                    if(event.button.button == SDL_BUTTON_LEFT)
                        if(!painting){
                            painting = 1;
                            logging.data("Painting: ", painting);
                            paint(gui);
                            
                        }
                $}case(SDL_MOUSEBUTTONUP):{
                    if(event.button.button == SDL_BUTTON_LEFT){
                        painting = 0;
                        logging.data("Painting: ", painting);
                    }
                $}case(SDL_MOUSEMOTION):{
                    gui->mouse.x = event.motion.x;
                    gui->mouse.y = event.motion.y;
                    if(painting)
                        paint(gui);
                $}default:$
            }
        }
        usleep(event_rate);       
    }

    pthread_exit(0);
}





void start_editor(void){

    modes.RUNNING = 1;
    logging.info("Begin program");
    
    gui_engine_t gui = gui_engine_init();

    init_events(&gui);



    u8 world_r, world_c;
    i32 ch_width = CHUNK_SIZE*CELL_RESOLUTION;
    i32 ch_origin = ch_width/2;

    SDL_Rect chunk_box;
    chunk_box.w = chunk_box.h = ch_width;
    
    hexcode_u color;

    i32 ch_origin_y, ch_origin_x;


    while(modes.RUNNING){

        get_dim(gui.sdl2.window, &gui.dim);

        clear_screen(gui.sdl2.renderer);

        pthread_mutex_lock(&origin_lock);

        ch_origin_y = gui.dim.h/2 - RENDER_DISTANCE*ch_origin;
        ch_origin_x = gui.dim.w/2 - RENDER_DISTANCE*ch_origin;


        for(world_r=0; world_r<RENDER_DISTANCE; world_r++){

            chunk_box.y = ch_origin_y + world_r*ch_width;

            for(world_c=0; world_c<RENDER_DISTANCE; world_c++){
                chunk_box.x = ch_origin_x + world_c*ch_width;

                if(world.array[world_r][world_c].defined){

                    draw_chunk(&gui, world_r, world_c, ch_origin_y, ch_origin_x);
                    
                }

                if(debug.chunk_borders){
                    color.code = 0xFF0000;
                    draw_rect(gui.sdl2.renderer, &chunk_box, color);
                }
            }
        }
        pthread_mutex_unlock(&origin_lock);


        SDL_RenderPresent(gui.sdl2.renderer);
        usleep(event_rate);
    }

    SDL_DestroyRenderer(gui.sdl2.renderer);
    SDL_DestroyWindow(gui.sdl2.window);
    SDL_Quit();
    return;
}


int main (void){
    boot_menu();
    logging.detail("This is the map editor\n\to - enable chunk borders\n\tp - enable cell borders\n\t# - terrain type");
    return 0;
}