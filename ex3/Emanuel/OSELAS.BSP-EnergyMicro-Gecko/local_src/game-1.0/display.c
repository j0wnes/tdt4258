#include "display.h"

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)


uint16_t *display;
int display_descriptor;

void setup_display() {
    display_descriptor = open("/dev/fb0", O_RDWR);
    if (display_descriptor == -1)
        handle_error("Failed opening display descriptor");

    display = (uint16_t*)mmap(NULL, DISPLAY_BYTES, PROT_READ | PROT_WRITE, MAP_SHARED, display_descriptor, 0);
    if (display == MAP_FAILED)
        handle_error("Failed memory mapping display");
}

void teardown_display() {
    munmap(display, DISPLAY_BYTES);
    close(display_descriptor);
}

void clear_rectangle(int x, int y, int width, int height) {
    draw_rectangle_color(x, y, width, height, BACKGROUND_COLOR);
}

void draw_rectangle(int x, int y, int width, int height) {
    draw_rectangle_color(x, y, width, height, GAME_COLOR);
}

void draw_rectangle_color(int x, int y, int width, int height, int color) {
    int i, j;
    for (i = y; i < (y + height); i++) {
        for (j = x; j < (x + width); j++) {
            display[i * DISPLAY_WIDTH + j] = color;
        }
    }

    // setup which part of the frame buffer that is to be refreshed
    // for performance reasons, use as small rectangle as possible
    struct fb_copyarea rect;
    rect.dx = x;
    rect.dy = y;
    rect.width = width;
    rect.height = height;

    // command driver to update display
    ioctl(display_descriptor, 0x4680, &rect);
}
