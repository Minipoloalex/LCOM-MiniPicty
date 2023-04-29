#include <stdio.h>
#include <lcom/lcf.h>

#include "../../devices/video_card/video_card.h"

int (setup_menu)();
void (setup_buttons)();
int (draw_buttons)();
void (draw_menu)();

struct Button {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint8_t color;
    char *text;
};

struct Button buttons[3];

