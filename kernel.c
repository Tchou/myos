/* Start of the text video memory */
unsigned short * vga_base_ptr = (unsigned short *) 0xb8000;
unsigned int LINE  = 0;
unsigned int COL = 0;


static inline void outb(unsigned short port, unsigned char val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline unsigned short vga_char (unsigned char c) {

        return ((unsigned short) c) | 0x0a00; /* green on black */
}

void left(void) {
        if (COL > 0) COL--;
}

void right(void) {
        if (COL < 79) COL++;
}
void up(void) {
        if (LINE > 0) LINE--;
}

void down(void) {
        if (LINE < 24) LINE++;
}
void bol (void) {
        COL = 0;
}
void eol (void) {
        COL = 24;
}

void update_cursor() {
        unsigned short pos = LINE * 80 + COL;
        outb(0x3d4, 0x0f);
        outb(0x3d5, (pos & 0xff));
        outb(0x3d4, 0x0e);
        outb(0x3d5, (pos >> 8) & 0xff);
}

void putc_at(unsigned char c, unsigned int line, unsigned int col) {
        LINE = line;
        COL = col;

        vga_base_ptr[LINE * 80 + COL] = vga_char(c);

}

void putc(unsigned char c) {

        if (c == '\n') {
                down ();
                bol ();
        } else {
                putc_at(c, LINE, COL);
                right();
        }

}

void puts(unsigned char* s) {

        while (*s != '\0') {
                putc(*s);
                s++;
        };
        update_cursor();
}
void clear_screen(void) {
        unsigned int i;
        unsigned int j;
        for (j = 0; j < 25; j++)
                for (i = 0; i < 80; i++)
                        putc_at(' ', j, i);
        COL=0;
        LINE=0;
        update_cursor();
}

void kernel_main(void) {
        unsigned int i = 0;
        clear_screen();
        for (i = 0; i < 10; i ++)
                puts("Oh my kernel !\n");
}
