#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "display.h"

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define BALL_SIZE 10
#define BALL_MOVEMENT_AMOUNT 1
#define PADDLE_HEIGHT 50
#define PADDLE_WIDTH 10
#define PADDLE_SIDE_MARGIN 20
#define PADDLE_MOVE_SIZE 20

enum ball_directions {NE, SE, SW, NW};

int gamepad_driver_descriptor;

int ball_pos_x;
int ball_pos_y;
int paddle_pos_p1;
int paddle_pos_p2;
int ball_direction;
int end_game = 0;

void redraw_paddles() {
    clear_rectangle(PADDLE_SIDE_MARGIN, 0, PADDLE_WIDTH, DISPLAY_HEIGHT);
    draw_rectangle(PADDLE_SIDE_MARGIN, paddle_pos_p1, PADDLE_WIDTH, PADDLE_HEIGHT);
    clear_rectangle(DISPLAY_WIDTH - PADDLE_SIDE_MARGIN - PADDLE_WIDTH, 0, PADDLE_WIDTH, DISPLAY_HEIGHT);
    draw_rectangle(DISPLAY_WIDTH - PADDLE_SIDE_MARGIN - PADDLE_WIDTH, paddle_pos_p2, PADDLE_WIDTH, PADDLE_HEIGHT);
}

void redraw_ball(int old_ball_pos_x, int old_ball_pos_y) {
    clear_rectangle(old_ball_pos_x, old_ball_pos_y, BALL_SIZE, BALL_SIZE);
    draw_rectangle(ball_pos_x, ball_pos_y, BALL_SIZE, BALL_SIZE);
    // printf("ball_pos_x: %d, ball_pos_y: %d \n", ball_pos_x, ball_pos_y);
}

int update_ball_pos() {
    int new_ball_pos_x = 0;
    int new_ball_pos_y = 0;
    switch (ball_direction) {
        case NE:
            new_ball_pos_x = ball_pos_x + BALL_MOVEMENT_AMOUNT;
            new_ball_pos_y = ball_pos_y - BALL_MOVEMENT_AMOUNT;
            break;
        case SE:
            new_ball_pos_x = ball_pos_x + BALL_MOVEMENT_AMOUNT;
            new_ball_pos_y = ball_pos_y + BALL_MOVEMENT_AMOUNT;
            break;
        case SW:
            new_ball_pos_x = ball_pos_x - BALL_MOVEMENT_AMOUNT;
            new_ball_pos_y = ball_pos_y + BALL_MOVEMENT_AMOUNT;
            break;
        case NW:
            new_ball_pos_x = ball_pos_x - BALL_MOVEMENT_AMOUNT;
            new_ball_pos_y = ball_pos_y - BALL_MOVEMENT_AMOUNT;
            break;
    }
    if (new_ball_pos_y < 0) {
        new_ball_pos_y = -new_ball_pos_y;
        ball_direction = ball_direction == NE ? SE : SW;
    } else if ((new_ball_pos_y + BALL_SIZE) > DISPLAY_HEIGHT) {
        new_ball_pos_y = (DISPLAY_HEIGHT - BALL_SIZE) - ((new_ball_pos_y + BALL_SIZE) - DISPLAY_HEIGHT);
        ball_direction = ball_direction == SE ? NE : NW;
    }
    int valid = 1;
    if (new_ball_pos_x < PADDLE_SIDE_MARGIN + PADDLE_WIDTH) {
        if ((new_ball_pos_y + BALL_SIZE) > paddle_pos_p1 && new_ball_pos_y < (paddle_pos_p1 + PADDLE_HEIGHT)) {
            valid = 1;
            new_ball_pos_x = PADDLE_SIDE_MARGIN + PADDLE_WIDTH + (PADDLE_SIDE_MARGIN + PADDLE_WIDTH - new_ball_pos_x);
            ball_direction = ball_direction == NW ? NE : SE;
        } else {
            valid = 0;
        }
    } else if ((new_ball_pos_x + BALL_SIZE) > (DISPLAY_WIDTH - PADDLE_SIDE_MARGIN - PADDLE_WIDTH)) {
        if ((new_ball_pos_y + BALL_SIZE) > paddle_pos_p2 && new_ball_pos_y < (paddle_pos_p2 + PADDLE_HEIGHT)) {
            valid = 1;
            new_ball_pos_x = (DISPLAY_WIDTH - PADDLE_SIDE_MARGIN - PADDLE_WIDTH - BALL_SIZE) - ((new_ball_pos_x + BALL_SIZE) - (DISPLAY_WIDTH - PADDLE_SIDE_MARGIN - PADDLE_WIDTH));
            ball_direction = ball_direction == NE ? NW : SW;
        } else {
            valid = 0;
        }
    }
    ball_pos_x = new_ball_pos_x;
    ball_pos_y = new_ball_pos_y;
    return valid;
}

int move_ball() {
    int old_ball_pos_x = ball_pos_x;
    int old_ball_pos_y = ball_pos_y;
    int valid = update_ball_pos();
    redraw_ball(old_ball_pos_x, old_ball_pos_y);
    return valid;
}

void game_loop() {
    int i = 0;
    while (!end_game) {
        if (i == 200000) {
            int valid = move_ball();
            if (!valid) return;
            i = 0;
        } else {
            i++;
        }
    }
}

void init_game() {
    clear_rectangle(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

    ball_pos_x = DISPLAY_WIDTH / 2 - BALL_SIZE / 2;
    ball_pos_y = DISPLAY_HEIGHT / 2 - BALL_SIZE / 2;
    redraw_ball(0, 0);

    paddle_pos_p1 = DISPLAY_HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddle_pos_p2 = paddle_pos_p1;
    redraw_paddles();

    ball_direction = NE;
    game_loop();
}

void set_end_game() {
    end_game = 1;
}

void move_paddle_up_p1() {
    if (paddle_pos_p1 > PADDLE_MOVE_SIZE){
        paddle_pos_p1 -= PADDLE_MOVE_SIZE;
    } else {
        paddle_pos_p1 = 0;
    }
    redraw_paddles();
}

void move_paddle_up_p2() {
    if (paddle_pos_p2 > PADDLE_MOVE_SIZE){
        paddle_pos_p2 -= PADDLE_MOVE_SIZE;
    } else {
        paddle_pos_p2 = 0;
    }
    redraw_paddles();
}

void move_paddle_down_p1() {
    if (paddle_pos_p1 < DISPLAY_HEIGHT - PADDLE_HEIGHT - PADDLE_MOVE_SIZE){
        paddle_pos_p1 += PADDLE_MOVE_SIZE;
    } else {
        paddle_pos_p1 = DISPLAY_HEIGHT - PADDLE_HEIGHT;
    }
    redraw_paddles();
}

void move_paddle_down_p2() {
    if (paddle_pos_p2 < DISPLAY_HEIGHT - PADDLE_HEIGHT - PADDLE_MOVE_SIZE){
        paddle_pos_p2 += PADDLE_MOVE_SIZE;
    } else {
        paddle_pos_p2 = DISPLAY_HEIGHT - PADDLE_HEIGHT;
    }
    redraw_paddles();
}

// Interrupt signal handler to respond to keypresses
void keypress_handler(int signal) {
    // printf("Keypress detected, reading input file...\n");
    char input_buff;
    if (read(gamepad_driver_descriptor, &input_buff, 1) != -1) { // Read exactly one byte
        // Read was successful
        // printf("Successfully read the input, data: %d\n", input_buff);
        // Filter input to let two players press buttons at once
        switch (input_buff) {
            case 2:
                move_paddle_up_p1();
                break;
            case 8:
                move_paddle_down_p1();
                break;
            case 32:
                move_paddle_up_p2();
                break;
            case 128:
                move_paddle_down_p2();
                break;
            case 4:
            case 64:
                set_end_game();
                break;
        }
    } else {
        printf("Failed to read input, contents of input_buff: %d\n", input_buff);
    }
}

void setup_gamepad() {
    gamepad_driver_descriptor = open("/dev/template", O_RDWR);
    if (gamepad_driver_descriptor == -1) {
        handle_error("Could not initialize gamepad driver");
    }

    if (signal(SIGIO, &keypress_handler) == SIG_ERR) {
        handle_error("Error: failed to create signal handler\n");
    }
    // Set correct ownership
    if (fcntl(gamepad_driver_descriptor, F_SETOWN, getpid()) == -1) {
        handle_error("Error: failed to set owner of gamepad device\n");
    }
    // Set ASYNC flag to be able to receive notifications
    long flags = fcntl(gamepad_driver_descriptor, F_GETFL);
    if (fcntl(gamepad_driver_descriptor, F_SETFL, flags | FASYNC) == -1) {
        handle_error("Error: failed to set FASYNC flag\n");
    }
}

void teardown_gamepad() {
    close(gamepad_driver_descriptor);
}


int main(int argc, char *argv[])
{
    printf("Starting game\n");

    setup_display();
    setup_gamepad();

    while (!end_game) {
        printf("New Game\n");
        init_game();
    }

    printf("Game ending\n");
    teardown_display();
    teardown_gamepad();
    exit(EXIT_SUCCESS);
}
