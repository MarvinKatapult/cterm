/*
    MIT License

    Copyright (c) 2024 Marvin Katapult

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the Software), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#ifndef __C_TERM__
#define __C_TERM__

#include <stddef.h>
#include <stdbool.h>

typedef enum CT_Color_t {
    CT_Default = 0,
    CT_Black,
    CT_Red,
    CT_Green,
    CT_Yellow,
    CT_Blue,
    CT_Magenta,
    CT_Cyan,
    CT_White,
    CT_LBlack,
    CT_LRed,
    CT_LGreen,
    CT_LYellow,
    CT_LBlue,
    CT_LMagenta,
    CT_LCyan,
    CT_LWhite
} CT_Color_t;

/** Bootstrap **/
void CT_init(void);
void CT_clean_up(void);

/** Utils **/
void CT_clear_screen(void);
bool CT_move_cursor(int x, int y);
void CT_set_color(CT_Color_t fg, CT_Color_t bg);
void CT_set_fg_color(CT_Color_t color);
void CT_set_bg_color(CT_Color_t color);
void CT_reset_color(void);
void CT_show_cursor(bool show);
bool CT_is_ctrl_char(int ch);
bool CT_is_out_of_bounds(int x, int y);
void CT_sleep(size_t secs);

/** Terminal Geometry **/
int CT_get_term_width(void);
int CT_get_term_height(void);
void CT_get_term_size(int * w, int * h);

/** Printing **/
void CT_put_str(const char * str);
void CT_put_str_ext(const char * str, CT_Color_t print_col, CT_Color_t bg_col);
bool CT_put_str_at(const char * str, int x, int y);
bool CT_put_str_at_ext(const char * str, int x, int y, CT_Color_t print_col, CT_Color_t bg_col);
void CT_fill_background(CT_Color_t color);
void CT_draw_rect(int x, int y, int width, int height, CT_Color_t print_col, CT_Color_t bg_col);
void CT_fill_rect(int x, int y, int width, int height, CT_Color_t bg_col);
void CT_update_buffer(void);

/** Rawmode **/
// From https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
void CT_disable_raw_mode(void);
void CT_enable_raw_mode(void);
unsigned char CT_read_term_input(void);

#endif // __C_TERM__
