/*
This is fun. Basically you give C the signature of the function from rust, but you do not allow them to actually access
the function or structure internals. This prevents them from fucking shit up on their end or breaking the natural
structure of rust programs methods
*/

#ifndef GQUEST_RUST_GCURSES_H
#define GQUEST_RUST_GCURSES_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif




typedef struct gc_term_t gc_term_t;
typedef struct gc_frame_t gc_frame_t;

typedef enum {
  GC_DEFAULT = 0,
  GC_BLACK = 1,
  GC_RED = 2,
  GC_GREEN = 3,
  GC_YELLOW = 4,
  GC_BLUE = 5,
  GC_MAGENTA = 6,
  GC_CYAN = 7,
  GC_WHITE = 8
} gc_color_t;


typedef struct {
  uint32_t glyph;
  gc_color_t fg_color;
  gc_color_t bg_color;
} gc_cell_t;


typedef struct {
  size_t width;
  size_t height;
  gc_cell_t cell;
} gc_rect_t;


/*
 * source must be a null-terminated array of gc_cell_t.
 * The terminating cell must have glyph == 0.
 */
typedef struct {
  const gc_cell_t *source;
} gc_fstr_t;


/* lifecycle */
gc_term_t *gc_new(void);
void gc_drop(gc_term_t *terminal);


/* terminal information */
size_t gc_nrows(const gc_term_t *terminal);
size_t gc_ncols(const gc_term_t *terminal);


/* frame operations */
void gc_clear(gc_term_t *terminal);
void gc_present(gc_term_t *terminal);
void gc_move_cursor(gc_term_t *terminal, size_t row, size_t col);
void gc_frame_resize(gc_term_t *terminal);


/* direct cell access */
void gc_set(gc_term_t *terminal, size_t row, size_t col, gc_cell_t cell);


/* cursor / input */
void gc_show_cursor(gc_term_t *terminal, bool setting);
void gc_io_block(gc_term_t *terminal, bool setting);
void gc_echo(gc_term_t *terminal, bool setting);

/* strings */
size_t gc_horz_str_disp(gc_term_t *terminal, size_t row, size_t col, const char *string);
size_t gc_vert_str_disp(gc_term_t *terminal, size_t row, size_t col, const char *string);


/* formatted strings */
void gc_horz_fstr_disp(gc_term_t *terminal,size_t row,size_t col,gc_fstr_t string);
void gc_vert_fstr_disp(gc_term_t *terminal,size_t row,size_t col,gc_fstr_t string);


/* shapes */
void gc_draw_rect(gc_term_t *terminal,size_t row,size_t col,gc_rect_t rect);
void gc_draw_border(gc_term_t *terminal,size_t row,size_t col,gc_rect_t rect);
void gc_draw_frame(gc_term_t *terminal,size_t row,size_t col,gc_rect_t rect);

/* frames */
gc_frame_t *gc_frame_new(void);
void gc_frame_free(gc_frame_t* frame);
size_t gc_frame_height(gc_frame_t* frame);
size_t gc_frame_width(gc_frame_t* frame);
void gc_set_frame_fg_color(gc_frame_t* frame, gc_color_t color);
void gc_set_frame_bg_color(gc_frame_t* frame, gc_color_t color);
void gc_frame_set_width(gc_frame_t* frame, size_t value);
void gc_frame_set_height(gc_frame_t* frame, size_t value);
void gc_frame_set_max_width(gc_frame_t* frame, size_t value);
void gc_frame_set_min_width(gc_frame_t* frame, size_t value);
void gc_frame_set_max_height(gc_frame_t* frame, size_t value);
void gc_frame_set_min_height(gc_frame_t* frame, size_t value);


#ifdef __cplusplus
}
#endif

#endif //GQUEST_RUST_GCURSES_H
