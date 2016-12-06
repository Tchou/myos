#include "utils.h"
#include "screen.h"

unsigned short *const VGA_BASE = (unsigned short*) 0xb8000;

#define MAX_ROW 25u
#define MAX_COL 80u



static inline unsigned short vga_char_color(unsigned char c, enum vga_color fg, enum vga_color bg) {
        unsigned short cc = (unsigned short) c;
        unsigned short color = (unsigned short) (fg | (bg << 4));
        return cc | (color << 8);
}


unsigned short SCREEN_BUFFER[ MAX_ROW * MAX_COL ] = {};

static enum vga_color FG_COLOR = VGA_COLOR_LIGHT_GREY;
static enum vga_color BG_COLOR = VGA_COLOR_BLACK;
static unsigned int CUR_ROW = 0;
static unsigned int CUR_COL = 0;

static inline unsigned short vga_char(unsigned char c) {
        return vga_char_color(c, FG_COLOR, BG_COLOR);
}

static inline unsigned int vga_offset(unsigned int row, unsigned int col) {
        return row * MAX_COL + col;
}

void clear_screen (void) {
        unsigned int i;
        for (i = 0; i < MAX_COL * MAX_ROW; i++)
                VGA_BASE[i] = vga_char(' ');
}

void put_char(unsigned char c) {

        unsigned short vga_c = vga_char(c);
        unsigned int i,j;
        char to_draw = 0;
        if (CUR_COL >= MAX_COL) {
                CUR_ROW++;
                CUR_COL = 0;
        };

        if (CUR_ROW >= MAX_ROW) {
                CUR_ROW--;
                for (j = 1; j < MAX_ROW; j++) {
                        for (i = 0; i < MAX_COL; i++) {
                                SCREEN_BUFFER[ vga_offset(j-1, i) ] =
                                        SCREEN_BUFFER[vga_offset(j,i)];
                        }
                }
                to_draw = -1;
        } else {
                to_draw = CUR_COL;
        }

        if (c == '\n') {
                vga_c = vga_char (' ');
                for (; CUR_COL < MAX_COL; CUR_COL++)
                        SCREEN_BUFFER[ vga_offset(CUR_ROW, CUR_COL) ] = vga_c;
        } else {
                SCREEN_BUFFER[ vga_offset(CUR_ROW, CUR_COL) ] = vga_c;

        }
        CUR_COL++;
        if (to_draw < 0) {
                //redraw the screen
                for (i = 0; i < MAX_COL * MAX_ROW; i++)
                        VGA_BASE[i] = SCREEN_BUFFER[i];
        } else {
                for (i = to_draw; i < MAX_COL; i++)
                        VGA_BASE[ vga_offset(CUR_ROW,i)] = SCREEN_BUFFER[vga_offset(CUR_ROW,i)];
        }

}

void clear_buffer () {
        unsigned int i;
        for (i = 0; i < MAX_ROW * MAX_COL; i ++) {
                SCREEN_BUFFER[i] = vga_char(' ');
        }

}

void put_string(unsigned char* c) {

        while (*c != '\0') {
                put_char(*c);
                c++;
        }
        set_cursor(CUR_ROW, CUR_COL);
}

void set_fg_color(enum vga_color c) {
        FG_COLOR = c;
}

void set_bg_color(enum vga_color c) {
        BG_COLOR = c;
}

void set_cursor(unsigned int row, unsigned int col) {
        unsigned short pos = vga_offset(row,col);
        outb(0x3d4, 0x0f);
        outb(0x3d5, (pos & 0xff));
        outb(0x3d4, 0x0e);
        outb(0x3d5, (pos >> 8) & 0xff);
}
