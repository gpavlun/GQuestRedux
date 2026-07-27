#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

#include <gio.h>
#include <gcurses.h>
#include <logging.h>

#include "dev.h"

#define nopts 7
#define optw 20
char options[nopts][optw]={
    "run game           ",
    "kill game          ",
    "build+run game     ",
    "run editor         ",
    "kill editor        ",
    "build+run editor   ",
    "quit               "
};
int readpid(const char *path){
    FILE *f;
    pid_t pid;

    while(1){
        f = fopen(path, "r");

        if(f){
            if(fscanf(f, "%d", &pid)==1){
                fclose(f);
                return pid;
            }
            fclose(f);
        }

        usleep(10000);
    }
}
void launch(launch_t *config){
    sprintf(config->tmp_path,"./tmp/%s.pid",config->name);
    int pid = fork();
    if (!pid) {
        execlp(config->app_term,
            config->app_term,
            "-e",
            "./bin/launcher-wrapper",
            config->tmp_path,
            config->app_path,
            NULL);

        exit(1);
    }
    config->pid = readpid(config->tmp_path);
}

void get_status(launch_t *config){
    FILE *f;
    if(config->tmp_path&&config->tmp_path[0]){
        f = fopen(config->tmp_path, "r");
        if(f){
            config->running = 1;
            fclose(f);
        }else{
            config->running = 0;
        }
    }else{
        config->running = 0;
    }
}

int handle_input(char input, char *out, int *selected, launch_t *config[2]){
    FILE *output;
    char buff[1];    
    
    get_status(config[0]);
    get_status(config[1]);


    if(input!=EOF && input!=0){
        start:
        switch(input){
            case('\n'):{
                input = *selected + 0x31;
                goto start;
            $}case('w'):{
                if((*selected) > 0) (*selected)--;
            $}case('s'):{
                if((*selected) < nopts-1) (*selected)++;
            $}case('1'):{
                *selected = 0;
                if(!config[0]->running){
                    launch(config[0]);
                    strcpy(out, "launched game");
                }else{
                    strcpy(out, "game is running");
                }
            $}case('2'):{
                *selected = 1;
                if(config[0]->running){
                    kill(config[0]->pid, SIGKILL);
                    remove(config[0]->tmp_path);
                    config[0]->tmp_path[0] = 0;
                    strcpy(out, "killed game");
                }else{
                    strcpy(out, "game is not running");
                }
            $}case('3'):{
                *selected = 2;
                if(!config[0]->running){
                    output = popen("./scripts/build-game.sh", "r");
                    if(output && fread(buff, 1, 1, output) > 0){
                        strcpy(out, "error building game");
                        system("./scripts/build-game.sh 1");
                    }else{
                        launch(config[0]);
                        strcpy(out, "launched game");
                    }
                    pclose(output);
                }else{
                    strcpy(out, "game is running");
                }
            $}case('4'):{
                *selected = 3;
                if(!config[1]->running){
                    launch(config[1]);
                    strcpy(out, "launched editor");
                }else{
                    strcpy(out, "editor is running");
                }
            $}case('5'):{
                *selected = 4;
                if(config[1]->running){
                    kill(config[1]->pid, SIGKILL);
                    remove(config[1]->tmp_path);
                    config[1]->tmp_path[0] = 0;
                    strcpy(out, "killed editor");
                }else{
                    strcpy(out, "editor is not running");
                }
            $}case('6'):{
                *selected = 5;
                if(!config[1]->running){
                    output = popen("./scripts/build-editor.sh", "r");
                    if(output && fread(buff, 1, 1, output) > 0){
                        strcpy(out, "error building editor");
                        system("./scripts/build-editor.sh 1");
                    }else{
                        launch(config[1]);
                        strcpy(out, "launched editor");
                    }
                    pclose(output);
                }else{
                    strcpy(out, "editor is running");
                }         
            $}case('7'):{
                *selected = 6;
                return 0;
            $}case(0x1B):{
                return 0;
            $}case('q'):{
                return 0;
            $}default:{
                strcpy(out, "not an option");
            $} 
        }
    }
    return 1;
}

void draw_display(gc_term_t *terminal, char *message,  int selected){

    gc_cell_t array[nopts][optw] = {0};

    gc_rect_t title_box = {0};
    title_box.cell.fg_color = GC_GREEN;
    title_box.cell.bg_color = GC_DEFAULT;
    gc_rect_t options_box = {0};
    options_box.cell.fg_color = GC_GREEN;
    options_box.cell.bg_color = GC_DEFAULT;
    gc_rect_t body_box = {0};
    body_box.cell.fg_color = GC_YELLOW;
    body_box.cell.bg_color = GC_DEFAULT;
    gc_rect_t cmd_box = {0};
    cmd_box.cell.fg_color = GC_BLUE;
    cmd_box.cell.bg_color = GC_DEFAULT;
  
    char *str;
    gc_fstr_t fstr = {0};

        gc_frame_resize(terminal);
        title_box.width = gc_ncols(terminal);
        title_box.height = 3;
        gc_draw_frame(terminal, 0 , 0, title_box);

        str  = "GQ DEVELOPER STUDIO";
        gc_horz_str_disp(terminal, 1,gc_ncols(terminal)/2 - strlen("GQ DEVELOPER STUDIO")/2, str);

        options_box.width = 21;
        options_box.height = gc_nrows(terminal) - title_box.height * 2;
        gc_draw_frame(terminal, title_box.height, 0, options_box);

        str = "1. w and s to select options";
        gc_horz_str_disp(terminal, title_box.height + 1, options_box.width + 1, str);
        str = "2. enter to run option";
        gc_horz_str_disp(terminal, title_box.height + 2, options_box.width + 1, str);
        str = "3. 1-9 are hotkeys for select";
        gc_horz_str_disp(terminal, title_box.height + 3, options_box.width + 1, str);
        str = "4. esc or q to quick exit";
        gc_horz_str_disp(terminal, title_box.height + 4, options_box.width + 1, str);

        body_box.height = gc_nrows(terminal) - title_box.height * 2;
        body_box.width = gc_ncols(terminal) - options_box.width;
        gc_draw_frame(terminal, title_box.height, options_box.width, body_box);

        cmd_box.height = 3;
        cmd_box.width = gc_ncols(terminal);
        gc_draw_frame(terminal, title_box.height + options_box.height, 0, cmd_box);

        str = ">";
        gc_horz_str_disp(terminal, title_box.height + options_box.height + 1, 2, str);
        
        str = "                      ";
        gc_horz_str_disp(terminal, title_box.height + options_box.height + 1, 4, str);
        str = message;
        gc_horz_str_disp(terminal, title_box.height + options_box.height + 1, 4, str);
        gc_move_cursor(terminal, title_box.height + options_box.height + 1, 4 + strlen(message));

        int j, i;
        for (j=0; j<nopts; j++) {

            for (i=0; options[j][i] != '\0'; i++) {

                if (selected == j) {
                    array[j][i].bg_color = GC_WHITE;
                    array[j][i].fg_color = GC_BLACK;
                } else {
                    array[j][i].bg_color = GC_DEFAULT;
                    array[j][i].fg_color = GC_DEFAULT;
                }
                array[j][i].glyph = (unsigned char)options[j][i];

            }

            array[j][i].bg_color = GC_BLACK;
            array[j][i].fg_color = GC_WHITE;
            array[j][i].glyph = 0;
            fstr.source = array[j];
            gc_horz_fstr_disp(terminal, title_box.height + 1 + j, 1, fstr);
        }
        gc_present(terminal);

}


void boot_menu(void){
    
    gc_term_t *terminal;
    terminal = gc_new();

    gc_io_block(terminal, 0);
    gc_echo(terminal, 0);
    gc_clear(terminal);
    gc_present(terminal);
    
    char input = EOF;
    char message[64];

    strcpy(message, "startup complete");

    int selected = 0;    
    int running = 1;    
    
    launch_t *config[2];
    config[0] = calloc(1, sizeof(launch_t));
    config[1] = calloc(1, sizeof(launch_t));
    FILE *file = fopen("launcher.conf", "r");
    if(!file) logging.error(404, "launcher.conf not found!");
    rewind(file);
    if(parse_config(file, "GAME", config[0])) logging.error(500, "Game config malformed");
    rewind(file);
    if(parse_config(file, "EDITOR", config[1])) logging.error(500, "Editor config malformed");

    while(running){

        running = handle_input(input, message, &selected, config);
        draw_display(terminal, message, selected);

        input = get_input();
        usleep(10000);
    }

    gc_move_cursor(terminal, gc_nrows(terminal), 0);
    gc_clear(terminal);
    gc_present(terminal);
    gc_drop(terminal);
}

int main(void){
    system("rm ./tmp/*");
    boot_menu();
    return 0;
}
