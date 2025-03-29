
#include "cterm.h"

#include <signal.h>
#include <stdio.h>

#include <string.h>

typedef struct {
    int x, y;
    int w, h;
    const char * text;
} Rect_t;

static bool should_close;

void handle_signals(int sig) {
    switch (sig) {
        case SIGINT:
            should_close = true;
            break;
    }
}

int main() {
    signal(SIGINT, handle_signals);
    CT_init();

    Rect_t rect = {10, 10, 30, 15, "This is a Button :)"};
    while (!should_close) {
        CT_fill_rect(rect.x, rect.y, rect.w, rect.h, CT_Yellow);
        CT_put_str_at_ext(rect.text, rect.x + rect.w / 2 - strlen(rect.text) / 2, rect.y + rect.h / 2, CT_Black, CT_Yellow);
        CT_update_buffer();
        CT_sleep(1);
    }
    CT_clear_screen();
    CT_update_buffer();
    CT_clean_up();

    return 0;
}
