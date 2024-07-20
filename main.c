
#include "cterm.h"

int main() {

    int w, h;
    getTermSize(&w, &h);
    fillScreenBg(CT_Red);
    putStrExt("Hello World", CT_Black, CT_White);
    putStrAtExt("Hey whats up", 10, 10, CT_LGreen, CT_White);
    putStr("\n");

    return 0;
}
