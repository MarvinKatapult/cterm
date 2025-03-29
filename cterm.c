#include "./cterm.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>

#include <ctype.h>

typedef struct CT_cell {
    CT_Color_t fg;
    CT_Color_t bg;
    bool changed;
    char c;
} CT_Cell_t;

static CT_Cell_t * draw_buffer;
static CT_Cell_t * current_cell;
static CT_Color_t current_fg_color;
static CT_Color_t current_bg_color;

void cSetFgColor(CT_Color_t color) {
    switch (color) {
        case CT_Default:  printf("\033[39m"); break;
        case CT_Black:    printf("\033[30m"); break;
        case CT_Red:      printf("\033[31m"); break;
        case CT_Green:    printf("\033[32m"); break;
        case CT_Yellow:   printf("\033[33m"); break;
        case CT_Blue:     printf("\033[34m"); break;
        case CT_Magenta:  printf("\033[35m"); break;
        case CT_Cyan:     printf("\033[36m"); break;
        case CT_White:    printf("\033[37m"); break;
        case CT_LBlack:   printf("\033[90m"); break;
        case CT_LRed:     printf("\033[91m"); break;
        case CT_LGreen:   printf("\033[92m"); break;
        case CT_LYellow:  printf("\033[93m"); break;
        case CT_LBlue:    printf("\033[94m"); break;
        case CT_LMagenta: printf("\033[95m"); break;
        case CT_LCyan:    printf("\033[96m"); break;
        case CT_LWhite:   printf("\033[97m"); break;
    }
}

void cSetBgColor(CT_Color_t color) {
    switch (color) {
        case CT_Default:  printf("\033[49m");  break;
        case CT_Black:    printf("\033[40m");  break;
        case CT_Red:      printf("\033[41m");  break;
        case CT_Green:    printf("\033[42m");  break;
        case CT_Yellow:   printf("\033[43m");  break;
        case CT_Blue:     printf("\033[44m");  break;
        case CT_Magenta:  printf("\033[45m");  break;
        case CT_Cyan:     printf("\033[46m");  break;
        case CT_White:    printf("\033[47m");  break;
        case CT_LBlack:   printf("\033[100m"); break;
        case CT_LRed:     printf("\033[101m"); break;
        case CT_LGreen:   printf("\033[102m"); break;
        case CT_LYellow:  printf("\033[103m"); break;
        case CT_LBlue:    printf("\033[104m"); break;
        case CT_LMagenta: printf("\033[105m"); break;
        case CT_LCyan:    printf("\033[106m"); break;
        case CT_LWhite:   printf("\033[107m"); break;
    }
}

void cMoveCursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}

void cPrintfAt(int x, int y, char c) {
    cMoveCursor(x, y);
    printf("%c", c);
}

static void resizeBuffer() {
    int w, h;
    CT_get_term_size(&w, &h);
    assert(w != 0 && h != 0);
    draw_buffer = realloc(draw_buffer, sizeof(CT_Cell_t) * w * h);
    assert(draw_buffer);
    printf("Resizing Buffer\n");
}

static void handleSignals(int sig) {
    switch (sig) {
        case SIGWINCH:
            resizeBuffer();
            break;
    }
}


void CT_init() {
    CT_show_cursor(false);
    signal(SIGWINCH, handleSignals);
    resizeBuffer();
    CT_clear_screen();
    current_cell = draw_buffer; 
}

void CT_clean_up() {
    CT_show_cursor(true);
    free(draw_buffer);
}

void CT_clear_screen() {
    int w, h;
    CT_get_term_size(&w, &h);
    for (int i = 0; i < w * h; i++) {
        CT_put_str_at_ext(" ", i % w, i / w, CT_Default, CT_Default);
    }
}

bool CT_move_cursor(int x, int y) {
    if (CT_is_out_of_bounds(x, y)) return false;
    const int w = CT_get_term_width();
    current_cell = &draw_buffer[y * w + x];
    return true;
}

void CT_set_color(CT_Color_t fg, CT_Color_t bg) {
    CT_set_fg_color(fg);
    CT_set_bg_color(bg);
}

void CT_set_fg_color(CT_Color_t color) {
    current_fg_color = color;
}

void CT_set_bg_color(CT_Color_t color) {
    current_bg_color = color;
}

void CT_reset_color(void) {
    CT_set_color(CT_Default, CT_Default);
}

void CT_show_cursor(bool show) {
    if (show) printf("\e[?25h"); 
    else      printf("\e[?25l");
}

bool CT_is_out_of_bounds(int x, int y) {
    int w, h;
    CT_get_term_size(&w, &h);
    return (y * w + x >= w * h);
}

void CT_sleep(size_t secs) {
    sleep(secs);
}

bool currentCellOutOfBounds() {
    int w, h;
    CT_get_term_size(&w, &h);
    if (current_cell - draw_buffer >= w * h) return true;
    return false;
}

void CT_put_str(const char * str) {
    while (*str) {
        current_cell->changed = true;
        current_cell->fg      = current_fg_color;
        current_cell->bg      = current_bg_color;
        (current_cell++)->c = *str++;
        if (currentCellOutOfBounds()) {
            assert(CT_move_cursor(0, 0));
        }
    }
}

void CT_put_str_ext(const char * str, CT_Color_t print_col, CT_Color_t bg_col) {
    CT_set_fg_color(print_col);
    CT_set_bg_color(bg_col);
    CT_put_str(str);
    CT_reset_color();
}

bool CT_put_str_at(const char * str, int x, int y) {
    if (!CT_move_cursor(x, y)) return false;
    CT_put_str(str);
    return true;
}

bool CT_put_str_at_ext(const char * str, int x, int y, CT_Color_t print_col, CT_Color_t bg_col) {
    CT_set_fg_color(print_col);
    CT_set_bg_color(bg_col);
    if (!CT_move_cursor(x, y)) return false;
    CT_put_str(str);
    CT_reset_color();
    return true;
}

int CT_get_term_width(void) {
    int w = 0;
    CT_get_term_size(&w, NULL);
    return w;
}

int CT_get_term_height(void) {
    int h = 0;
    CT_get_term_size(NULL, &h);
    return h;
}

void CT_get_term_size(int * w, int * h) {
    struct winsize win;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
    if (w) *w = win.ws_col;
    if (h) *h = win.ws_row;
}

void CT_fill_background(CT_Color_t color) {
    CT_move_cursor(0, 0);
    CT_set_bg_color(color);
    int w, h;
    CT_get_term_size(&w, &h);
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            CT_put_str(" ");
        }
    }
    CT_move_cursor(0, 0);
}

void CT_draw_rect(int x, int y, int width, int height, CT_Color_t fg, CT_Color_t bg) {
    if (x < 0 || y < 0 || width <= 1 || height <= 1) return;
    CT_set_color(fg, bg);

    for (int i = 1; i < width - 1; i++) {
        CT_put_str_at("-", x + i, y);
        CT_put_str_at("-", x + i, y + height - 1);
    }
    for (int i = 1; i < height - 1; i++) {
        CT_put_str_at("|", x, y + i);
        CT_put_str_at("|", x + width - 1, y + i);
    }
    CT_put_str_at("#", x, y);
    CT_put_str_at("#", x + width - 1, y);
    CT_put_str_at("#", x, y + height - 1);
    CT_put_str_at("#", x + width - 1, y + height - 1);
    CT_reset_color();
}

void CT_fill_rect(int x, int y, int width, int height, CT_Color_t bg) {
    for (int i = x; i < x + width; i++) {
        for (int j = y; j < y + height; j++) {
            CT_put_str_at_ext(" ", i, j, CT_Black, bg);
        }
    }
}

void CT_update_buffer(void) {
    int w, h;
    CT_get_term_size(&w, &h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            CT_Cell_t * cell = &draw_buffer[y * w + x];
            if (!cell->changed) continue;
            cSetFgColor(cell->fg);
            cSetBgColor(cell->bg);
            cPrintfAt(x, y, cell->c);
            cell->changed = false;
        }
    }
    fflush(stdout);
    cMoveCursor(0, 0);
}

static struct termios orig_termios;

// From https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
void CT_disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void CT_enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(CT_disable_raw_mode);
    struct termios raw = orig_termios;
    raw.c_iflag &= ~(ICRNL | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

bool CT_is_ctrl_char(int ch) {
    return iscntrl(ch);
}

unsigned char CT_read_term_input(void) {
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    return c;
}
