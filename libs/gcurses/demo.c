#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "gcurses.h"

void handleinput(int input, int *selected){
    switch(input){
        case 'w':
            if((*selected) > 0) (*selected)--;
            break;
        case 's':
            if((*selected) < 5) (*selected)++;
            break;
    }
}



int getch_nb(void)
{
    unsigned char c;
    size_t n = read(STDIN_FILENO, &c, 1);
    if (n == 1)           return (int)c;
    if (n == 0 || (n == -1)) return -1; 
    return -2;                          
}

int main(void) {

    // term_w_t terminal;
    // GC_init_tui(&terminal);
    // terminal.io_block(0);
    // terminal.cursor(0);

    // char text[10];
    // char input = 0;

    // cursor_origin();

    // terminal.clear();

    // int selected = 0;


    // tframe_t title_box;
    // GC_init_tframe(&title_box);

    // title_box.set.max_w(strlen("GCURSES DEMO")*2 + 2);
    // title_box.set.min_w(strlen("GCURSES DEMO") + 2);

    // title_box.set.h(3);
    // title_box.dim.tile.color = GCS_BLUE;

    // rect_t body_box;
    // body_box.tile.color = GCS_RED;
    // body_box.tile.bg_color = GCS_BG_DEFAULT;
    // rect_t ui_box;
    // ui_box.tile.color = GCS_GREEN;
    // ui_box.tile.bg_color = GCS_BG_DEFAULT;

    // str_t str;
    // fstr_t fstr;
    // while(input!='q'&&input!='\e'){
    //     terminal.frame_resize();

    //     handleinput(input, &selected);

    //     title_box.set.w(terminal.ncols/4);


    //     terminal.draw_frame(&title_box.dim);


    //     str.r = title_box.dim.r+1;
    //     str.c = title_box.dim.c+1;
    //     str.source = "GCURSES DEMO";
    //     terminal.horz_strdisp(&str);

    //     body_box.c = title_box.dim.c;
    //     body_box.r = title_box.dim.r+title_box.dim.h;
    //     body_box.h = terminal.nrows-title_box.dim.h;
    //     body_box.w = title_box.dim.w;

    //     terminal.draw_frame(&body_box);

    //     ui_box.c = title_box.dim.c+title_box.dim.w;
    //     ui_box.r = title_box.dim.r;
    //     ui_box.h = terminal.nrows;
    //     ui_box.w = terminal.ncols-title_box.dim.w;

    //     terminal.draw_frame(&ui_box);

    //     str.r = ui_box.r+1;
    //     str.c = ui_box.c+1;
    //     str.source = "This is a demo of gcurses, my implementation of ncurses.";
    //     terminal.horz_strdisp(&str);

    //     str.r = body_box.r+2;
    //     str.c = body_box.c+body_box.w-2;
    //     str.source = "Vertical string";
    //     terminal.vert_strdisp(&str);


    //     str.r = ui_box.r+2;
    //     str.c = ui_box.c+1;
    //     str.source = "Dynamic resizing is enabled, try moving the window around!";
    //     terminal.horz_strdisp(&str);

    //     str.r = ui_box.r+3;
    //     str.c = ui_box.c+1;
    //     str.source = "There are also min and max widths, see the title box!";
    //     terminal.horz_strdisp(&str);

    //     str.r = ui_box.r+6;
    //     str.c = ui_box.c+1;
    //     str.source = "Press q or esc to exit.";
    //     terminal.horz_strdisp(&str);


    //     tile_t array[6][16] = {0};

    //     char set[6][16] = {
    //         "option 0",
    //         "option 1",
    //         "option 2",
    //         "option 3",
    //         "option 4",
    //         "option 5"
    //     };



    //     int j, i;
    //     for (j=0; j<6; j++) {
    //         for (i=0; set[j][i] != '\0'; i++) {

    //             if (selected == j) {
    //                 array[j][i].bg_color = GCS_BG_WHITE;
    //                 array[j][i].color = GCS_BLACK;
    //             } else {
    //                 array[j][i].bg_color = GCS_BG_DEFAULT;
    //                 array[j][i].color = GCS_DEFAULT;
    //             }
    //             array[j][i].symbol[0] = set[j][i];
    //             array[j][i].symbol[1] = '\0';
    //         }
    //         array[j][i].bg_color = GCS_BG_BLACK;
    //         array[j][i].color = GCS_WHITE;
    //         array[j][i].symbol[0] = 0;

    //     }
    //     for(int i=0;i<6;i++){
    //         fstr.r = body_box.r+1+i;
    //         fstr.c = body_box.c+1;
    //         fstr.source = array[i];
    //         terminal.horz_tiledisp(&fstr);
    //     }




    //     terminal.present();

    //     input = getch_nb();
    //     usleep(10000);
    // }

    // terminal.clear();
    // terminal.present();
    // terminal.io_block(1);
    // terminal.cursor(1);

    // return 0;
}
/*
//rust main
fn main() -> StdResult<()> {
    let mut terminal = term_window_t::new()?;

    let _restore = restore::new()?;

    terminal.io_block(false)?;
    hide_cursor!();
    cursor_origin!();

    terminal.clear();

    let selected: usize = 2;
    let mut input = '\0';

    let mut title_box = frame_t::new();

    title_box.set_max_width("GCURSES DEMO".len() * 2 + 2);
    title_box.set_min_width("GCURSES DEMO".len() + 2);
    title_box.set_height(3);

    title_box.dimension.cell.fg_color = color_t::Blue;


    let mut body_box = rect_t {
        width: 0,
        height: 0,
        cell: cell_t {
            glyph: ' ',
            fg_color: color_t::Red,
            bg_color: color_t::Default,
        },
    };

    let mut ui_box = rect_t {
        width: 0,
        height: 0,
        cell: cell_t {
            glyph: ' ',
            fg_color: color_t::Green,
            bg_color: color_t::Default,
        },
    };


    while input != 'q' && input != '\x1b' {

        terminal.frame_resize()?;


        title_box.set_width(
            terminal.ncols / 4
        );


        terminal.draw_frame(
            0,
            0,
            title_box.dimension
        );


        terminal.horz_str_disp(
            1,
            1,
            "GCURSES DEMO"
        );


        body_box.width = title_box.dimension.width;
        body_box.height =
            terminal.nrows - title_box.dimension.height;


        terminal.draw_frame(
            title_box.dimension.height,
            0,
            body_box
        );


        ui_box.width =
            terminal.ncols - title_box.dimension.width;

        ui_box.height =
            terminal.nrows;


        terminal.draw_frame(
            0,
            title_box.dimension.width,
            ui_box
        );


        terminal.horz_str_disp(
            1,
            title_box.dimension.width + 1,
            "This is a demo of gcurses, my implementation of ncurses."
        );


        terminal.horz_str_disp(
            2,
            title_box.dimension.width + 1,
            "Dynamic resizing is enabled, try moving the window around!"
        );


        terminal.horz_str_disp(
            3,
            title_box.dimension.width + 1,
            "There are also min and max widths, see the title box!"
        );


        terminal.horz_str_disp(
            6,
            title_box.dimension.width + 1,
            "Press q or esc to exit."
        );


        let options = [
            "option 0",
            "option 1",
            "option 2",
            "option 3",
            "option 4",
            "option 5",
        ];


        for (i, option) in options.iter().enumerate() {

            let (fg, bg) = if selected == i {
                (
                    color_t::Black,
                    color_t::White,
                )
            } else {
                (
                    color_t::Default,
                    color_t::Default,
                )
            };


            let line = fstr_t::from_str(
                option,
                fg,
                bg,
            );


            terminal.horz_fstr_disp(
                title_box.dimension.height + 1 + i,
                1,
                &line,
            );
        }


        terminal.present();


        input = 's';

        std::thread::sleep(
            std::time::Duration::from_millis(10)
        );
    }


    terminal.clear();
    terminal.present();

    terminal.io_block(true)?;
    show_cursor!();

    Ok(())
}
*/


 */