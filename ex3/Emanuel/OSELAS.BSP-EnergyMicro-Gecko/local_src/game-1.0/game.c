#include <stdio.h>
#include <stdlib.h>
#include "display.h"


int main(int argc, char *argv[])
{
    printf("Hello World, I'm a modified game!\n");

    setup_display();

    draw_rectangle(40, 40, 50, 50);
    draw_rectangle(180, 180, 50, 50);

    printf("Seeing a rectangle?\n");

    teardown_display();
    exit(EXIT_SUCCESS);
}
