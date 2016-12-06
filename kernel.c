#include "screen.h"
#include "utils.h"
void kernel_main(void) {
        unsigned int i = 0;
        set_fg_color(VGA_COLOR_RED);
        set_bg_color(VGA_COLOR_WHITE);
        clear_screen();
        set_cursor(0,0);
        for (i = 0; i < 0xffffffff; i ++) {
                put_string("Oh my kernel ! ");
                put_string(itoa(i));
                put_string("\n");
        }
}
