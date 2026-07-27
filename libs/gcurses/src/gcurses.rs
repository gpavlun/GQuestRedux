use std::io::{stdin, stdout, Write};
use std::io::Result as StdResult;

use rustix::termios;
use rustix::fd::AsFd;
use rustix::fs::{fcntl_getfl, fcntl_setfl, OFlags};



macro_rules! cursor_origin {
    () => {
        print!("\x1b[H")
    };
}

macro_rules! hide_cursor {
    () => {
        print!("\x1b[?25l")
    };
}

macro_rules! show_cursor {
    () => {
        print!("\x1b[?25h")
    };
}

pub struct fstr_t {
    pub source: Vec<cell_t>,
}
impl fstr_t {
    pub fn from_str(text: &str, fg: color_t, bg: color_t) -> Self {
        let source = text
            .chars()
            .map(|ch| cell_t {
                glyph: ch,
                fg_color: fg,
                bg_color: bg,
            })
            .collect();

        Self { source }
    }
}


#[repr(u8)]
#[derive(Clone, Copy, PartialEq, Eq)]
pub enum color_t {
    Default = 0,
    Black = 1,
    Red = 2,
    Green = 3,
    Yellow = 4,
    Blue = 5,
    Magenta = 6,
    Cyan = 7,
    White = 8,
}
fn print_fg_color(color: color_t){
    match color {
        color_t::Default => print!("\x1b[39m"),
        color_t::Black   => print!("\x1b[30m"),
        color_t::Red     => print!("\x1b[31m"),
        color_t::Green   => print!("\x1b[32m"),
        color_t::Yellow  => print!("\x1b[33m"),
        color_t::Blue    => print!("\x1b[34m"),
        color_t::Magenta => print!("\x1b[35m"),
        color_t::Cyan    => print!("\x1b[36m"),
        color_t::White   => print!("\x1b[37m"),
    }
}
fn print_bg_color(color: color_t){
    match color {
        color_t::Default => print!("\x1b[49m"),
        color_t::Black   => print!("\x1b[40m"),
        color_t::Red     => print!("\x1b[41m"),
        color_t::Green   => print!("\x1b[42m"),
        color_t::Yellow  => print!("\x1b[43m"),
        color_t::Blue    => print!("\x1b[44m"),
        color_t::Magenta => print!("\x1b[45m"),
        color_t::Cyan    => print!("\x1b[46m"),
        color_t::White   => print!("\x1b[47m"),
    }

}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct rect_t{
    pub width: usize,
    pub height: usize,
    pub cell: cell_t
}
pub struct frame_t{
    pub max_width: usize,
    pub max_height: usize,
    pub min_width: usize,
    pub min_height: usize,
    pub dimension: rect_t
}

impl frame_t {
    pub fn new() -> Self {
        Self {
            max_width: 0,
            max_height: 0,
            min_width: 0,
            min_height: 0,
            dimension: rect_t {
                width: 0,
                height: 0,
                cell: cell_t {
                    glyph: 'X',
                    fg_color: color_t::Default,
                    bg_color: color_t::Default,
                },
            },
        }
    }

    pub fn width(&self) -> usize {
        return self.dimension.width;
    }

    pub fn height(&self) -> usize {
        return self.dimension.height;
    }

    pub fn set_fg_color(&mut self, code: color_t){
        self.dimension.cell.fg_color = code;
    }

    pub fn set_bg_color(&mut self, code: color_t){
        self.dimension.cell.bg_color = code;
    }

    pub fn set_width(&mut self, value: usize) {
        if self.max_width > 0 {
            if value < self.max_width && value >= self.min_width {
                self.dimension.width = value;
            } else if value < self.min_width {
                self.dimension.width = self.min_width;
            } else {
                self.dimension.width = self.max_width;
            }
        } else {
            if value >= self.min_width {
                self.dimension.width = value;
            } else {
                self.dimension.width = self.min_width;
            }
        }
    }

    pub fn set_height(&mut self, value: usize) {
        if value >= self.min_height {
            self.dimension.height = value;
        } else {
            self.dimension.height = self.min_height;
        }

        if self.max_height > 0 {
            if value < self.max_height {
                self.dimension.height = value;
            } else {
                self.dimension.height = self.max_height;
            }
        }
    }


    pub fn set_max_width(&mut self, value: usize) {
        self.max_width = value;
    }


    pub fn set_min_width(&mut self, value: usize) {
        self.min_width = value;
    }


    pub fn set_max_height(&mut self, value: usize) {
        self.max_height = value;
    }


    pub fn set_min_height(&mut self, value: usize) {
        self.min_height = value;
    }
}

#[derive(Clone, Copy, PartialEq)]
pub struct cell_t{
    pub glyph: char,
    pub fg_color: color_t,
    pub bg_color: color_t
}
pub struct cursor_t{
    row: usize,
    col: usize,
    visible: bool
}

pub struct term_window_t{
    nrows: usize,
    ncols: usize,
    array: Vec<cell_t>,
    prev: Vec<cell_t>,
    cursor: cursor_t,
    original_term: termios::Termios
}

impl term_window_t {
    pub fn new()->Result<Self, rustix::io::Errno>{
        
        let mut stdout = stdout();
        let original_term = termios::tcgetattr(stdout.as_fd())?;
        let dim = termios::tcgetwinsize(stdout.as_fd())?;
        let nrows = dim.ws_row as usize;
        let ncols = dim.ws_col as usize;

        cursor_origin!();
        let cursor: cursor_t = cursor_t { 
            row: 0, 
            col: 0, 
            visible: true 
        };

        let blank: cell_t = cell_t{
            glyph: ' ',
            fg_color: color_t::Default,
            bg_color: color_t::Default,
        };

        let array = vec![blank; nrows * ncols];


        for cell in &array{
                print!("{}", cell.glyph);
        }

        print!("\x1b[0m");
        cursor_origin!();

        stdout.flush().unwrap();
        let prev:Vec<cell_t> = array.clone();

        Ok(Self { 
            nrows,
            ncols, 
            array,
            prev,
            cursor,
            original_term
        })
    }
    pub fn ncols(&self) -> usize {
        return self.ncols;
    }
    pub fn nrows(&self) -> usize {
        return self.nrows;
    }
    pub fn clear(&mut self){
        let blank: cell_t = cell_t {
            glyph: ' ',
            fg_color: color_t::Default,
            bg_color: color_t::Default,
        };

        self.array.fill(blank);
    }

    pub fn move_cursor(&mut self, row: usize, col: usize){
        self.cursor.row = row;
        self.cursor.col = col;
        print!("\x1b[{};{}H", row+1, col+1);
    }

    pub fn echo(setting: bool) -> Result<(), rustix::io::Errno>{
        let stdout = stdout();
        let mut tio = termios::tcgetattr(stdout.as_fd())?;
        if setting {
            tio.local_modes.insert(termios::LocalModes::ECHO);
        }else{
            tio.local_modes.remove(termios::LocalModes::ECHO);
        }
        termios::tcsetattr(stdout.as_fd(), termios::OptionalActions::Now, &tio)?;
        Ok(())
    }

    pub fn present(&mut self){

        if self.cursor.visible {hide_cursor!()};
        cursor_origin!();

        let mut selected_fg_color = color_t::Default;
        let mut selected_bg_color = color_t::Default;

        for i in 0..(self.nrows*self.ncols){
            if self.array[i] != self.prev[i]{
                print!("\x1b[{};{}H", i/self.ncols+1, i%self.ncols+1);
                if self.array[i].bg_color != selected_bg_color {
                    selected_bg_color = self.array[i].bg_color;
                    print_bg_color(selected_bg_color);
                }
                if self.array[i].fg_color != selected_fg_color {
                    selected_fg_color = self.array[i].fg_color;
                    print_fg_color(selected_fg_color);
                }
                print!("{}", self.array[i].glyph);
            }
        }

        print!("\x1b[0m");

        print!("\x1b[{};{}H", self.cursor.row+1, self.cursor.col+1);
        if self.cursor.visible {show_cursor!()};

        stdout().flush().unwrap();
        self.prev.clone_from_slice(&self.array);
    }

    pub fn set(&mut self, row: usize, col: usize ,cell: cell_t){
        if row >= self.nrows || col >= self.ncols { return; }
        self.array[row*self.ncols + col] = cell;
    }

    pub fn show_cursor(&mut self, setting: bool){
        if setting {
            show_cursor!();
            self.cursor.visible = true;
        }else{
            hide_cursor!();
            self.cursor.visible = false;
        }

    }

    pub fn raw_input_mode() -> Result<(), rustix::io::Errno> {
        let input = stdin();
        let fd = input.as_fd();

        let mut term = termios::tcgetattr(fd)?;

        // Disable canonical mode and echo
        term.local_modes.remove(
            termios::LocalModes::ICANON |
            termios::LocalModes::ECHO
        );

        // Non-canonical read behavior:
        // return immediately, even with no input
        term.special_codes[termios::SpecialCodeIndex::VMIN] = 0;
        term.special_codes[termios::SpecialCodeIndex::VTIME] = 0;

        termios::tcsetattr(
            fd,
            termios::OptionalActions::Now,
            &term
        )?;

        Ok(())
    }    

    pub fn io_block(&self, setting: bool) -> StdResult<()> {
        let input = stdin();
        let flags = fcntl_getfl(input.as_fd())?;

        if setting {
            fcntl_setfl(input.as_fd(),
                        flags & !OFlags::NONBLOCK,)?;
        }else{
            fcntl_setfl(input.as_fd(),
                        flags | OFlags::NONBLOCK,)?;
        }
        Ok(())
    }

    pub fn canon(setting: bool) -> StdResult<()> {
        let input = stdin();
        let fd = input.as_fd();

        let mut term = termios::tcgetattr(fd)?;

        if setting {
            term.local_modes.insert(termios::LocalModes::ICANON);
            term.special_codes[termios::SpecialCodeIndex::VMIN] = 1;
            term.special_codes[termios::SpecialCodeIndex::VTIME] = 0;
        } else {
            term.local_modes.remove(termios::LocalModes::ICANON);
            term.special_codes[termios::SpecialCodeIndex::VMIN] = 0;
            term.special_codes[termios::SpecialCodeIndex::VTIME] = 1;
        }

        termios::tcsetattr(fd, termios::OptionalActions::Now, &term)?;

        Ok(())
    }

    pub fn horz_str_disp(&mut self, row: usize, col: usize, string: &str) -> usize{
        let mut count: usize = 0;
        if row >= self.nrows {
            return 0;
        }

        for (index, ch) in string.chars().enumerate() {
            let c = col + index;
            if c < self.ncols {
                let cell: cell_t = cell_t{
                    glyph: ch,
                    fg_color: color_t::Default,
                    bg_color: color_t::Default,
                };
                self.set(row, c, cell);
            }else{
                return count;
            }
            count = index + 1;
        }
        return count;
    }

    pub fn vert_str_disp(&mut self, row: usize, col: usize, string: &str) -> usize{
        let mut count: usize = 0;
        if col >= self.ncols {
            return 0;
        }

        for (index, ch) in string.chars().enumerate() {
            let r = row + index;
            if r < self.nrows {
                let cell: cell_t = cell_t{
                    glyph: ch,
                    fg_color: color_t::Default,
                    bg_color: color_t::Default,
                };
                self.set(r, col, cell);
            }else{
                return count;
            }
            count = index + 1;
        }
        return count;
    }

    pub fn horz_fstr_disp(&mut self, row: usize, col: usize, string: &fstr_t) -> usize{
        let mut count: usize = 0;
        if row >= self.nrows {
            return 0;
        }

        for (index, cell) in string.source.iter().enumerate() {
            let c = col + index;
            if c < self.ncols {
                self.set(row, c, *cell);
            }
            count = index + 1;
        }
        return count;
    }

    pub fn vert_fstr_disp(&mut self, row: usize, col: usize, string: &fstr_t) -> usize{
        let mut count: usize = 0;
        if col >= self.ncols {
            return 0;
        }

        for (index, cell) in string.source.iter().enumerate() {
            let r = row + index;
            if r < self.nrows {
                self.set(r, col, *cell);
            }
            count = index + 1;
        }
        return count;
    }

    pub fn draw_rect(&mut self, row: usize, col: usize, rect: rect_t){
        for r in row..(row + rect.height) {
            if r >= self.nrows { break; }
            for c in col..(col + rect.width) {
                if c >= self.ncols { break; }
                self.set(r, c, rect.cell);
            }
        }
    }

    pub fn draw_border(&mut self, row: usize, col: usize, rect: rect_t){
        if rect.width == 0 || rect.height == 0 ||
           row >= self.nrows || col >= self.ncols {
            return;
        }
        let row_edge = row + rect.height - 1;
        let col_edge = col + rect.width - 1;
        for r in row..=row_edge {
            self.set(r, col, rect.cell);
            self.set(r, col_edge, rect.cell);
        }
        for c in col..=col_edge {
            self.set(row, c, rect.cell);
            self.set(row_edge, c, rect.cell);
        }
    }

    pub fn draw_frame(&mut self, row: usize, col: usize, rect: rect_t) {
        if rect.width < 2 || rect.height < 2 {
            return;
        }

        let row_edge = row + rect.height - 1;
        let col_edge = col + rect.width - 1;

        let mut cell = rect.cell;

        cell.glyph = '┌';
        self.set(row, col, cell);

        cell.glyph = '┐';
        self.set(row, col_edge, cell);

        cell.glyph = '└';
        self.set(row_edge, col, cell);

        cell.glyph = '┘';
        self.set(row_edge, col_edge, cell);

        cell.glyph = '─';
        for i in 1..rect.width - 1 {
            self.set(row, col + i, cell);
            self.set(row_edge, col + i, cell);
        }

        cell.glyph = '│';
        for i in 1..rect.height - 1 {
            self.set(row + i, col, cell);
            self.set(row + i, col_edge, cell);
        }
    }

    pub fn detect_resize(&self) -> StdResult<bool> {
        let size = termios::tcgetwinsize(stdout().as_fd())?;

        Ok(self.nrows != size.ws_row as usize ||
            self.ncols != size.ws_col as usize)
    }

    pub fn frame_resize(&mut self) -> StdResult<()> {
        if !self.detect_resize()? {
            return Ok(());
        }

        while self.detect_resize()? {
            print!("\x1b[2J");
            stdout().flush()?;

            let size = rustix::termios::tcgetwinsize(stdout().as_fd())?;

            self.nrows = size.ws_row as usize;
            self.ncols = size.ws_col as usize;

            std::thread::sleep(std::time::Duration::from_millis(125));
        }

        let blank = cell_t {
            glyph: ' ',
            fg_color: color_t::Default,
            bg_color: color_t::Default,
        };

        let count = self.nrows * self.ncols;

        self.array = vec![blank; count];
        self.prev = vec![blank; count];

        Ok(())
    }

}
impl Drop for term_window_t {
    fn drop(&mut self) {
        let stdout = stdout();
        let _ = termios::tcsetattr(
            stdout.as_fd(),
            termios::OptionalActions::Now,
            &self.original_term,
        );
    }
}
