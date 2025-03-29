
#include "cterm.h"

#include <signal.h>

static bool should_close;

void handle_signals(int sig) {
    switch (sig) {
        case SIGINT:
            should_close = true;
            break;
    }
}

int main() {

    int count = 5;
    signal(SIGINT, handle_signals);
    CT_init();
    while (!should_close) {
        CT_draw_rect(5, 5, 10, 10, CT_White, CT_Red);
        CT_put_str_at("Hello", 5, count++);
        CT_update_buffer();
        CT_sleep(1);
    }
    CT_clean_up();

    return 0;
}
