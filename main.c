
#include "cterm.h"

#include <unistd.h>

struct Ball_t {
    int x, y;
    int x_vel, y_vel;
} ball;

int main() {
    CT_init();
    CT_enable_raw_mode();
    int player_pos = 2;
    ball = (struct Ball_t){2, 2, 1, 1};

    while (1) {
        char c = CT_read_term_input();
        if (c == 'q') break;

        CT_clear_screen();
        switch (c) {
            case 'a':
                player_pos--;
                break;
            case 'd':
                player_pos++;
                break;
        }

        ball.x += ball.x_vel;
        ball.y += ball.y_vel;

        CT_fill_rect(player_pos, CT_get_term_height() - 5, 10, 3, CT_White);
        CT_fill_rect(ball.x, ball.y, 2, 2, CT_White);
        CT_update_buffer();
    }
    CT_clear_screen();
    CT_update_buffer();
    CT_disable_raw_mode();
    CT_clean_up();

    return 0;
}
