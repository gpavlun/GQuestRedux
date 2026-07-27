use std::ffi::CStr;
use std::os::raw::c_char;

#[repr(C)]
#[derive(Clone, Copy)]
pub struct gc_cell_t {
    pub glyph: u32,
    pub fg_color: gc_color_t,
    pub bg_color: gc_color_t,
}
impl From<gc_cell_t> for cell_t {
    fn from(cell: gc_cell_t) -> Self {
        Self {
            glyph: char::from_u32(cell.glyph).unwrap_or(' '),
            fg_color: cell.fg_color.into(),
            bg_color: cell.bg_color.into(),
        }
    }
}
impl From<gc_rect_t> for rect_t {
    fn from(r: gc_rect_t) -> Self {
        rect_t {
            width: r.width,
            height: r.height,
            cell: r.cell.into(),
        }
    }
}
#[repr(C)]
#[derive(Clone, Copy)]
pub struct gc_rect_t {
    pub width: usize,
    pub height: usize,
    pub cell: gc_cell_t,
}

#[repr(C)]
#[derive(Clone, Copy)]
pub struct gc_fstr_t {
    pub source: *const gc_cell_t,
}
impl gc_fstr_t {
    pub unsafe fn to_fstr(&self) -> fstr_t {
        let mut source = Vec::new();

        if self.source.is_null() {
            return fstr_t { source };
        }

        let mut index = 0;

        loop {
            unsafe {
                let cell = *self.source.add(index);

                if cell.glyph == 0 {
                    break;
                }

                source.push(cell.into());
                index += 1;
            }
        }

        fstr_t { source }
    }
}
#[repr(C)]
#[derive(Clone, Copy, PartialEq, Eq)]
pub enum gc_color_t {
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
impl From<gc_color_t> for color_t {
    fn from(color: gc_color_t) -> Self {
        match color {
            gc_color_t::Default => color_t::Default,
            gc_color_t::Black => color_t::Black,
            gc_color_t::Red => color_t::Red,
            gc_color_t::Green => color_t::Green,
            gc_color_t::Yellow => color_t::Yellow,
            gc_color_t::Blue => color_t::Blue,
            gc_color_t::Magenta => color_t::Magenta,
            gc_color_t::Cyan => color_t::Cyan,
            gc_color_t::White => color_t::White,
        }
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_new() -> *mut term_window_t {
    match term_window_t::new() {
        Ok(term) => Box::into_raw(Box::new(term)),
        Err(_) => std::ptr::null_mut(),
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_move_cursor(term: *mut term_window_t, row: usize, col: usize){
    if term.is_null() {
        return;
    }

    unsafe {
        (*term).move_cursor(row, col);
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_echo(term: *mut term_window_t, setting: bool){
    if term.is_null() {
        return;
    }

    let _ = term_window_t::echo(setting);
    
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_canon(term: *mut term_window_t, setting: bool){
    if term.is_null() {
        return;
    }

    let _ = term_window_t::canon(setting);    
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_raw_input(term: *mut term_window_t){
    if term.is_null() {
        return;
    }

    let _ = term_window_t::raw_input_mode();    
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_drop(term: *mut term_window_t) {
    if term.is_null() {
        return;
    }

    unsafe {
        drop(Box::from_raw(term));
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_nrows(term: *const term_window_t) -> usize {
    if term.is_null() {
        return 0;
    }
    unsafe {
        return (*term).nrows();
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_ncols(term: *const term_window_t) -> usize {
    if term.is_null() {
        return 0;
    }
    unsafe {
        return (*term).ncols();
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_clear(term: *mut term_window_t) {
    if term.is_null() {return;}
    unsafe {
        (*term).clear();
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_present(term: *mut term_window_t) {
    if term.is_null() {return;}
    unsafe {
        (*term).present();
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_set(term: *mut term_window_t, row: usize, col: usize ,cell: gc_cell_t) {
    if term.is_null() {return;}
    unsafe {
        (*term).set(row, col, cell.into());
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_show_cursor(term: *mut term_window_t, setting: bool) {
    if term.is_null() {return;}
    unsafe {
        (*term).show_cursor(setting);
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_io_block(term: *mut term_window_t, setting: bool) {
    if term.is_null() {return;}
    unsafe {
        let _ = (*term).io_block(setting);
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_horz_str_disp(term: *mut term_window_t,
                                   row: usize, col: usize, string: *const c_char) -> usize {
    if term.is_null() {return 0;}
    if string.is_null() {return 0;}
    unsafe {
        let r_string = match CStr::from_ptr(string).to_str() {
            Ok(r_string) => r_string,
            Err(_) => return 0,
        };
        return (*term).horz_str_disp(row, col, r_string);
    }

}
#[unsafe(no_mangle)]
pub extern "C" fn gc_vert_str_disp(term: *mut term_window_t,
                                   row: usize, col: usize, string: *const c_char) -> usize {
    if term.is_null() {return 0;}
    if string.is_null() {return 0;}
    unsafe {
        let r_string = match CStr::from_ptr(string).to_str() {
            Ok(r_string) => r_string,
            Err(_) => return 0,
        };
        return (*term).vert_str_disp(row, col, r_string);
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_horz_fstr_disp(term: *mut term_window_t,
                                    row: usize, col: usize, string: gc_fstr_t) {
    if term.is_null(){return;}
    unsafe {
        let fstr = string.to_fstr();
        (*term).horz_fstr_disp(row, col, &fstr);
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn gc_vert_fstr_disp(term: *mut term_window_t,
                                    row: usize, col: usize, string: gc_fstr_t) {
    if term.is_null(){return;}
    unsafe {
        let fstr = string.to_fstr();

        (*term).vert_fstr_disp(row, col, &fstr);
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_draw_rect(term: *mut term_window_t,
                                row: usize, col: usize, rect: gc_rect_t,) {
    if term.is_null() {return;}
    unsafe {
        (*term).draw_rect(row, col, rect.into());
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_draw_border(term: *mut term_window_t,
                               row: usize, col: usize, rect: gc_rect_t,) {
    if term.is_null() {return;}
    unsafe {
        (*term).draw_border(row, col, rect.into());
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_draw_frame(term: *mut term_window_t,
                                row: usize, col: usize, rect: gc_rect_t,) {
    if term.is_null() {return;}
    unsafe {
        (*term).draw_frame(row, col, rect.into());
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_resize(term: *mut term_window_t) {
    if term.is_null() {return;}
    unsafe {
        let _ = (*term).frame_resize();
    }
}


#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_new() -> *mut frame_t {
    Box::into_raw(Box::new(frame_t::new()))
}

#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_free(frame: *mut frame_t) {
    unsafe {
        if !frame.is_null() {
            drop(Box::from_raw(frame));
        }
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_set_fg_color(frame: *mut frame_t, value: gc_color_t) {
    unsafe {
        if let Some(frame) = frame.as_mut() {
            frame.set_fg_color(value.into());
        }
    }
}
#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_set_bg_color(frame: *mut frame_t, value: gc_color_t) {
    unsafe {
        if let Some(frame) = frame.as_mut() {
            frame.set_bg_color(value.into());
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_set_width(frame: *mut frame_t, value: usize) {
    unsafe {
        if let Some(frame) = frame.as_mut() {
            frame.set_width(value);
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_set_height(frame: *mut frame_t, value: usize) {
    unsafe {
        if let Some(frame) = frame.as_mut() {
            frame.set_height(value);
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_set_max_width(frame: *mut frame_t, value: usize) {
    unsafe {
        if let Some(frame) = frame.as_mut() {
            frame.set_max_width(value);
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_set_min_width(frame: *mut frame_t, value: usize) {
    unsafe {
        if let Some(frame) = frame.as_mut() {
            frame.set_min_width(value);
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_set_max_height(frame: *mut frame_t, value: usize) {
    unsafe {
        if let Some(frame) = frame.as_mut() {
            frame.set_max_height(value);
        }
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn gc_frame_set_min_height(frame: *mut frame_t, value: usize) {
    unsafe {
        if let Some(frame) = frame.as_mut() {
            frame.set_min_height(value);
        }
    }
}