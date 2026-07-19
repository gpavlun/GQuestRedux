#include <SDL2/SDL.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>


#include <gio.h>
#include <logging.h>
#include <gcurses.h>

#include "editor.h"

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;


#define def typedef 


boolean_t modes;
debug_t debug;

world_t world;
pthread_mutex_t origin_lock;


int origin_chunk_x;
int origin_chunk_y;

uint32_t selected_terrain;
uint8_t painting;






def struct shared_data{
    float dist;

}data_t;
data_t sh;


def struct pos_data{
    float x;
    float y;
    float z;
}pos_t;
def struct player_data{
    pos_t pos;
    pos_t vel;
    pos_t acc;
    pos_t applied_force;
    float max_force;
    float mass;
}player_t;

def struct object_data{
    pos_t pos;
    pos_t rel;
    dimen_t dim;
}object_t;

player_t player; 
object_t object_glob;


typedef struct inputs{
    u8 w:1;
    u8 a:1;
    u8 s:1;
    u8 d:1;
    u8 space:1;
}inputs_t;

#define DRAG_CONSTANT 200
#define planetary_mass 10000000000000000000000000
#define planitary_diam 12756000
#define gravitational_const 0.000000000067408
#define cheat_grav 9.8

void game_logic(float dt){

    
    pos_t drag_force;
    drag_force.x = -(DRAG_CONSTANT * player.vel.x);
    drag_force.z = -(DRAG_CONSTANT * player.vel.z);
    
    double gravity_force;
    gravity_force = -cheat_grav * player.mass;
    

    
    pos_t net_force;
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
    if(player.pos.y<0){
        player.pos.y = 0;

        if(player.vel.y < 0)
            player.vel.y = 0;
    }
    
}



void multipress(inputs_t key){

    player.applied_force.x = player.applied_force.y = player.applied_force.z = 0;

    if( key.w ){
        player.applied_force.z = player.max_force;
    }
    if( key.s ){
        player.applied_force.z = -player.max_force/1.8;
    }
    if( key.a ){
        player.applied_force.x = -player.max_force/1.8;
    }
    if( key.d ){
        player.applied_force.x = player.max_force/1.8;
    }
    if( key.space && player.pos.y==0.0f ){
        player.applied_force.y = player.max_force*3.5;
    }
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
                        case(SDLK_ESCAPE):{ // window ctrls
                            modes.RUNNING = 0;
                        $}default:{$}
                    
                    
                    }
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


void draw_dis(gui_engine_t *gui, object_t object){
    window_t *window = &gui->window;
    pos_t cam_rel;
    SDL_Rect shape;


    hexcode_u color;

    shape.x = window->cen.x;
    shape.y = window->cen.y;
    shape.w = object.dim.w;
    shape.h = object.dim.h;
    float focal = 400;


    cam_rel.x = object.pos.x - player.pos.x;
    cam_rel.y = object.pos.y - player.pos.y;
    cam_rel.z = object.pos.z - player.pos.z;

    if (cam_rel.z <= 0.01f) return;

    float screen_x = cam_rel.x / cam_rel.z;
    float screen_y = cam_rel.y / cam_rel.z;

    shape.w = (int)(object.dim.w * focal / cam_rel.z);
    shape.h = (int)(object.dim.h * focal / cam_rel.z);

    shape.x = window->cen.x + (int)(screen_x * focal) - shape.w/2;
    shape.y = window->cen.y - (int)(screen_y * focal) - shape.h/2;



    sh.dist = sqrt( pow(object.pos.x - player.pos.x, 2) + 
                    pow(object.pos.y - player.pos.y, 2) +
                    pow(object.pos.z - player.pos.z, 2));

    color.code = 0xFFFFFF;
    set_color(gui->sdl2.renderer, color);
    SDL_RenderFillRect(gui->sdl2.renderer, &shape);

    color.code = 0x0000FF;
    set_color(gui->sdl2.renderer, color);
    SDL_RenderDrawRect(gui->sdl2.renderer, &shape);

}




void start_render(void){

    gui_engine_t gui = gui_engine_init();
    init_events(&gui);


    modes.RUNNING = 1;
  
    hexcode_u color;
    render_frame_t render;
    window_t *window = &gui.window;

    
    player.pos.x = 0;
    player.pos.y = 0;

    object_t object1, object2, object3;
    object1.pos.x = 0;
    object1.pos.y = 0;
    object1.pos.z = 50;
    object1.dim.w = 10;
    object1.dim.h = 20;

    object2.pos.x = 50;
    object2.pos.y = 0;
    object2.pos.z = 100;
    object2.dim.w = 10;
    object2.dim.h = 20;

    object3.pos.x = -20;
    object3.pos.y = 0;
    object3.pos.z = 20;
    object3.dim.w = 10;
    object3.dim.h = 20;

    while(modes.RUNNING){

        get_dim(gui.sdl2.window, window);

        clear_screen_wrap(gui.sdl2.renderer);


        draw_dis(&gui, object1);
        draw_dis(&gui, object2);
        draw_dis(&gui, object3);

        SDL_RenderPresent(gui.sdl2.renderer);
        usleep(event_rate);
    }

    SDL_DestroyRenderer(gui.sdl2.renderer);
    SDL_DestroyWindow(gui.sdl2.window);
    SDL_Quit();
    return;
}




void *tui(void *temp){
    
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

    while(*running){

        terminal.clear();

        str.c = 1;
        str.source = source;


        str.r = 1;
        sprintf(source,"player");
        terminal.horz_strdisp(str);

        str.r = 2;
        sprintf(source,"    [s] : (%.2f m, %.2fm, %.2fm)", player.pos.x, player.pos.y, player.pos.z);
        terminal.horz_strdisp(str);

        str.r = 3;
        sprintf(source,"    [v] : (%.2f ms, %.2fms, %.2fms)", player.vel.x, player.vel.y, player.vel.z);
        terminal.horz_strdisp(str);

        str.r = 4;
        sprintf(source,"    [a] : (%.2f ms^2, %.2fms^2, %.2fms^2)", player.acc.x, player.acc.y,  player.acc.z);
        terminal.horz_strdisp(str);

        str.r = 5;
        sprintf(source,"    applied force : (%.2f N, %.2fN, %.2fN)", player.applied_force.x, player.applied_force.y,  player.applied_force.z);
        terminal.horz_strdisp(str);




        str.r = 6;
        sprintf(source,"object");
        terminal.horz_strdisp(str);

        str.r = 7;
        //sprintf(source,"pos [%.2f,%.2f,%.2f] ",object_glob.pos.x, object_glob.pos.y, object_glob.pos.z);
        //terminal.horz_strdisp(str);

        str.r = 8;
        //sprintf(source,"cam_rel [%.2f,%.2f,%.2f] ",cam_rel.x, cam_rel.y, cam_rel.z);
        //terminal.horz_strdisp(str);

        str.r = 9;
        //sprintf(source,"dim [%d,%d] ",shape.w, shape.h);
        //terminal.horz_strdisp(str);

        str.r = 10;
        sprintf(source,"dist %.2f",sh.dist);
        terminal.horz_strdisp(str);        



        terminal.present();

        usleep(10000);
    }    


    terminal.clear();
    terminal.present();
    terminal.io_block(1);
    terminal.cursor(1);
    move_cursor(&terminal, terminal.nrows, 0); 
    pthread_exit(0);  
}







int main (void){

    

    
    int running = 1;
    pthread_t tui_thread;
    pthread_create(&tui_thread, NULL, tui, &running);

    start_render();
    //pthread_t logic_thread;
    //pthread_create(&logic_thread, NULL, game_logic, &running);



    running = 0;
    usleep(100000);
    pthread_cancel(tui_thread);
    pthread_join(tui_thread, NULL);
    return 0;
}