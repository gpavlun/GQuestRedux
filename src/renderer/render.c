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






def shared_data{
    float dist;

}data_t;
data_t sh;


def pos_data{
    float x;
    float y;
    float z;
}pos_t;
def player_data{
    pos_t pos;
    pos_t vel;
    pos_t acc;
    pos_t applied_force;
    float max_force;
    float mass;
}player_t;

def object_data{
    pos_t pos;
    pos_t rel;
    dimen_t dim;
}object_t;

player_t player; 
object_t object_glob;




def vertice_data{
    float x, y, z;
}vert_t;
def edge_data{
    u32 A, B;
}edge_t;
def triangle_data{
    u16 a,b,c;
}triangle_t;
def mesh_data{
    u16 nedges;
    edge_t *edges;
    u16 nverts;
    vert_t *verts;
    u16 ntrangs;
    triangle_t *trangs;
    hexcode_u color;
}mesh_t;



def inputs{
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


#define sensitivity 0.5

float theta;
float phi;






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
    float input_x = 0, input_z = 0;


    if( key.w ){
        input_z += 1;
    }
    if( key.s ){
        input_z -= 1;
    }
    if( key.a ){
        input_x -= 1;
    }
    if( key.d ){
        input_x += 1;
    }
    if( key.space && player.pos.y==0.0f ){
        player.applied_force.y = player.max_force*3.5;
    }
    float world_x;
    float world_z;

    world_x = input_x*cos(theta) + input_z*sin(theta);
    world_z = -input_x*sin(theta) + input_z*cos(theta);

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
    player.max_force = 2500;


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
                $}case(SDL_MOUSEMOTION):{

                    theta += event.motion.xrel * 0.01f;
                    phi -= event.motion.yrel * 0.01f;

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
    pos_t relative;
    SDL_Rect shape;


    hexcode_u color;

    shape.x = window->cen.x;
    shape.y = window->cen.y;
    shape.w = object.dim.w;
    shape.h = object.dim.h;
    float focal = 400;


    relative.x = object.pos.x - player.pos.x;
    relative.y = object.pos.y - player.pos.y;
    relative.z = object.pos.z - player.pos.z;

    if (relative.z <= 0.01f) return;

    float screen_x = relative.x / relative.z;
    float screen_y = relative.y / relative.z;

    shape.w = (int)(object.dim.w * focal / relative.z);
    shape.h = (int)(object.dim.h * focal / relative.z);

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
def vis_point_data{
    int x, y;
    u8 visible;
}point_t;

int max(int a, int b, int c)
{
    if(a > b && a > c) return a;
    if(b > a && b > c) return b;
    return c;
}

int min(int a, int b, int c)
{
    if(a < b && a < c) return a;
    if(b < a && b < c) return b;
    return c;
}



void draw_mesh_wireframe(gui_engine_t *gui, mesh_t mesh){//, float theta
    window_t *window = &gui->window;
    pos_t relative;
    pos_t rotated;
    pos_t pitched;
    float focal = 400;
    float screen_x, screen_y;
    

    hexcode_u color;
    color.code = 0xFF0000;
    set_color(gui->sdl2.renderer, color);

    //SDL_Rect verts_vis[mesh.nverts];
    point_t points[mesh.nverts];
    for(int i=0; i<mesh.nverts; i++){
        relative.x = mesh.verts[i].x - player.pos.x;
        relative.y = mesh.verts[i].y - player.pos.y;
        relative.z = mesh.verts[i].z - player.pos.z;        
        
        rotated.x = relative.x*cos(theta) - relative.z*sin(theta);
        rotated.y = relative.y;
        rotated.z = relative.x*sin(theta) + relative.z*cos(theta);

        pitched.x = rotated.x;
        pitched.y = rotated.y*cos(phi) - rotated.z*sin(phi);
        pitched.z = rotated.y*sin(phi) + rotated.z*cos(phi);  

        if (pitched.z <= 0.01f){
            points[i].visible = 0;
            continue;
        }
        points[i].visible = 1;

        
        screen_x = pitched.x / pitched.z;
        screen_y = pitched.y / pitched.z;

        //verts_vis[i].w = verts_vis[i].h = (int)(1 * focal / rotated.z);
        //verts_vis[i].x = window->cen.x + (int)(screen_x * focal);
        //verts_vis[i].y = window->cen.y - (int)(screen_y * focal);    
    
        //SDL_RenderFillRect(gui->sdl2.renderer, &verts_vis[i]);
        
        points[i].x = window->cen.x + (int)(screen_x * focal);
        points[i].y = window->cen.y - (int)(screen_y * focal);   
    }

    for(int i=0; i<mesh.nedges;i++){
        if(points[mesh.edges[i].A].visible && points[mesh.edges[i].B].visible){
            SDL_RenderDrawLine(gui->sdl2.renderer, points[mesh.edges[i].A].x, 
                            points[mesh.edges[i].A].y, 
                            points[mesh.edges[i].B].x, 
                            points[mesh.edges[i].B].y);
        }
    }
}
float edge(point_t a, point_t b, float x, float y)
{
    return (x-a.x)*(b.y-a.y)
         - (y-a.y)*(b.x-a.x);
}

void draw_mesh_triangle(gui_engine_t *gui, mesh_t mesh){
    window_t *window = &gui->window;
    pos_t relative;
    pos_t rotated;
    pos_t pitched;
    float focal = 400;

    float screen_x, screen_y;
    set_color(gui->sdl2.renderer, mesh.color);
    point_t points[mesh.nverts];

    for(int i=0; i<mesh.nverts; i++){
        relative.x = mesh.verts[i].x - player.pos.x;
        relative.y = mesh.verts[i].y - player.pos.y;
        relative.z = mesh.verts[i].z - player.pos.z;        
        
        rotated.x = relative.x*cos(theta) - relative.z*sin(theta);
        rotated.y = relative.y;
        rotated.z = relative.x*sin(theta) + relative.z*cos(theta);

        pitched.x = rotated.x;
        pitched.y = rotated.y*cos(phi) - rotated.z*sin(phi);
        pitched.z = rotated.y*sin(phi) + rotated.z*cos(phi);  

        if (pitched.z <= 0.01f){
            points[i].visible = 0;
            continue;
        }
        points[i].visible = 1;

        screen_x = pitched.x / pitched.z;
        screen_y = pitched.y / pitched.z;
        
        points[i].x = window->cen.x + (int)(screen_x * focal);
        points[i].y = window->cen.y - (int)(screen_y * focal);
    }



    for(int i=0; i<mesh.ntrangs; i++){

        if(!points[mesh.trangs[i].a].visible ||
        !points[mesh.trangs[i].b].visible ||
        !points[mesh.trangs[i].c].visible)
        {
            continue;
        }     
        
        
        int min_x = min(points[mesh.trangs[i].a].x,points[mesh.trangs[i].b].x,points[mesh.trangs[i].c].x);
        int max_x = max(points[mesh.trangs[i].a].x,points[mesh.trangs[i].b].x,points[mesh.trangs[i].c].x);    
        
        int min_y = min(points[mesh.trangs[i].a].y,points[mesh.trangs[i].b].y,points[mesh.trangs[i].c].y);
        int max_y = max(points[mesh.trangs[i].a].y,points[mesh.trangs[i].b].y,points[mesh.trangs[i].c].y);          
        
        if(min_x < 0){
            min_x = 0;
        }
            

        if(min_y < 0){
            min_y = 0;
        }
            
        if(max_x >= window->dim.w){
            max_x = window->dim.w - 1;
        }
            

        if(max_y >= window->dim.h){
            max_y = window->dim.h - 1;
        }
            
        
        float e1,e2,e3;
        for(int y=min_y; y<max_y; y++){
            for(int x=min_x; x<max_x; x++){
                e1 = edge(points[mesh.trangs[i].a],points[mesh.trangs[i].b],x,y);
                e2 = edge(points[mesh.trangs[i].b],points[mesh.trangs[i].c],x,y);
                e3 = edge(points[mesh.trangs[i].c],points[mesh.trangs[i].a],x,y);  
                if((e1 >= 0 && e2 >= 0 && e3 >= 0) ||
                (e1 <= 0 && e2 <= 0 && e3 <= 0)){
                    SDL_RenderDrawPoint(gui->sdl2.renderer, x, y);
                }
            }
        }
    }
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

    object_t object1;
    object1.pos.x = 0;
    object1.pos.y = 0;
    object1.pos.z = 50;
    object1.dim.w = 10;
    object1.dim.h = 20;



    mesh_t mesh1;
    mesh1.nverts = 8;
    mesh1.verts = malloc(sizeof(vert_t)*mesh1.nverts);
    
    mesh1.verts[0].x = 0;
    mesh1.verts[0].y = -10;
    mesh1.verts[0].z = 50;
    
    mesh1.verts[1].x = 20;
    mesh1.verts[1].y = -10;
    mesh1.verts[1].z = 80;

    mesh1.verts[2].x = 40;
    mesh1.verts[2].y = -10;
    mesh1.verts[2].z = 50;

    mesh1.verts[3].x = 20;
    mesh1.verts[3].y = -10;
    mesh1.verts[3].z = 30;

    mesh1.verts[4].x = 0;
    mesh1.verts[4].y = 10;
    mesh1.verts[4].z = 50;
    
    mesh1.verts[5].x = 20;
    mesh1.verts[5].y = 10;
    mesh1.verts[5].z = 80;

    mesh1.verts[6].x = 40;
    mesh1.verts[6].y = 10;
    mesh1.verts[6].z = 50;

    mesh1.verts[7].x = 20;
    mesh1.verts[7].y = 10;
    mesh1.verts[7].z = 30;


    mesh1.nedges = 12;
    mesh1.edges = malloc(sizeof(edge_t)*mesh1.nedges);

    mesh1.edges[0].A = 0;
    mesh1.edges[0].B = 1;

    mesh1.edges[1].A = 1;
    mesh1.edges[1].B = 2;

    mesh1.edges[2].A = 2;
    mesh1.edges[2].B = 3;

    mesh1.edges[3].A = 3;
    mesh1.edges[3].B = 0;

    mesh1.edges[4].A = 4;
    mesh1.edges[4].B = 5;

    mesh1.edges[5].A = 5;
    mesh1.edges[5].B = 6;

    mesh1.edges[6].A = 6;
    mesh1.edges[6].B = 7;

    mesh1.edges[7].A = 7;
    mesh1.edges[7].B = 4;

    mesh1.edges[8].A = 0;
    mesh1.edges[8].B = 4;

    mesh1.edges[9].A = 1;
    mesh1.edges[9].B = 5;

    mesh1.edges[10].A = 2;
    mesh1.edges[10].B = 6;

    mesh1.edges[11].A = 3;
    mesh1.edges[11].B = 7;    

















    mesh_t ground;
    ground.nverts = 4;
    ground.verts = malloc(sizeof(vert_t)*ground.nverts);
    
    ground.verts[0].x = -100;
    ground.verts[0].y = -10;
    ground.verts[0].z = -100;
    
    ground.verts[1].x = -100;
    ground.verts[1].y = -10;
    ground.verts[1].z = 100;

    ground.verts[2].x = 100;
    ground.verts[2].y = -10;
    ground.verts[2].z = -100;

    ground.verts[3].x = 100;
    ground.verts[3].y = -10;
    ground.verts[3].z = 100;    

    ground.nedges = 4;
    ground.edges = malloc(sizeof(edge_t)*ground.nedges);

    ground.edges[0].A = 0;
    ground.edges[0].B = 1;

    ground.edges[1].A = 1;
    ground.edges[1].B = 2;

    ground.edges[2].A = 2;
    ground.edges[2].B = 3;

    ground.edges[3].A = 3;
    ground.edges[3].B = 0;

    ground.ntrangs = 2;
    ground.trangs = malloc(sizeof(triangle_t)*ground.nedges);  
    
    ground.trangs[0].a = 0;
    ground.trangs[0].b = 1;
    ground.trangs[0].c = 2;

    ground.trangs[1].a = 1;
    ground.trangs[1].b = 3;
    ground.trangs[1].c = 2;

    ground.color.code = 0x00FF00;

    while(modes.RUNNING){

        get_dim(gui.sdl2.window, window);

        clear_screen_wrap(gui.sdl2.renderer);

        
        draw_mesh_triangle(&gui, ground);
        draw_mesh_wireframe(&gui, mesh1);

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
        sprintf(source,"theta %.2f :: phi %.2f", theta, phi);
        terminal.horz_strdisp(str);


        str.r = 10;
        sprintf(source,"error message: %s",glob_error);
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