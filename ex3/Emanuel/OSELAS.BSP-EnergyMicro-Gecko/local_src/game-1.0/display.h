#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>


#define DISPLAY_HEIGHT 240
#define DISPLAY_WIDTH 320
#define DISPLAY_PIXELS 240*320
#define DISPLAY_BYTES DISPLAY_PIXELS*2
#define GAME_COLOR 0xffff


void setup_display();
void teardown_display();
void draw_rectangle(int x, int y, int width, int height);
