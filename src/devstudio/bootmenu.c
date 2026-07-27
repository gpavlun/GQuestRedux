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
    "build game     ",
    "run editor         ",
    "kill editor        ",
    "build editor   ",
    "quit               "
};

int handle_input(char input, char *out, int *selected){
    FILE *output;
    char buff[1];    

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
                system("./scripts/run-game.sh");
                strcpy(out, "launched game");
            $}case('2'):{
                *selected = 1;
                system("tmux kill-window -t Game 2>/dev/null");
            $}case('3'):{
                *selected = 2;
                output = popen("./scripts/build-game.sh", "r");
                if(output && fread(buff, 1, 1, output) > 0){
                    strcpy(out, "error building game");
                    system("./scripts/build-game.sh 1");
                }else{
                    system("./scripts/run-game.sh");
                    strcpy(out, "launched game");
                }
                pclose(output);
            $}case('4'):{
                *selected = 3;
                system("./scripts/run-editor.sh");
                strcpy(out, "launched editor");
            $}case('5'):{
                *selected = 4;
                system("tmux kill-window -t Editor 2>/dev/null");
            $}case('6'):{
                *selected = 5;
                output = popen("./scripts/build-editor.sh", "r");
                if(output && fread(buff, 1, 1, output) > 0){
                    strcpy(out, "error building editor");
                    system("./scripts/build-editor.sh 1");
                }else{
                    system("./scripts/run-editor.sh");
                    strcpy(out, "launched editor");
                }
                pclose(output);
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
size_t sat_sub(size_t a, size_t b){
    return (a > b) ? (a - b) : 0;
}
void draw_display(gc_term_t *terminal, char *message,  int selected){

    gc_cell_t array[nopts][optw] = {0};

    gc_rect_t title_box = {0};
    title_box.cell.fg_color = GC_BLUE;
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
    gc_horz_str_disp(terminal, 1, sat_sub(gc_ncols(terminal)/2, strlen("GQ DEVELOPER STUDIO")/2), str);

    options_box.width = 21;
    options_box.height = sat_sub(gc_nrows(terminal), title_box.height * 2);
    gc_draw_frame(terminal, title_box.height, 0, options_box);

    str = "1. w and s to select options";
    gc_horz_str_disp(terminal, title_box.height + 1, options_box.width + 1, str);
    str = "2. enter to run option";
    gc_horz_str_disp(terminal, title_box.height + 2, options_box.width + 1, str);
    str = "3. 1-9 are hotkeys for select";
    gc_horz_str_disp(terminal, title_box.height + 3, options_box.width + 1, str);
    str = "4. F1-5 to select a window, or click the name in the toolbar";
    gc_horz_str_disp(terminal, title_box.height + 4, options_box.width + 1, str);
    str = "5. esc or q to quick exit";
    gc_horz_str_disp(terminal, title_box.height + 5, options_box.width + 1, str);
    body_box.height = sat_sub(gc_nrows(terminal), title_box.height * 2);
    body_box.width = sat_sub(gc_ncols(terminal), options_box.width);
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
    gc_canon(terminal, 0);
    
    gc_clear(terminal);
    gc_present(terminal);
    
    char input = EOF;
    char message[64];

    strcpy(message, "startup complete");

    int selected = 0;    
    int running = 1;    
    

    while(running){

        input = get_input();

        if(input) running = handle_input(input, message, &selected);
        draw_display(terminal, message, selected);

        //usleep(10000);
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
