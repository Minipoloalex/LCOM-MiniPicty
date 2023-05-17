#pragma once

#include <lcom/lcf.h>
#include "../position/position.h"

struct button {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint8_t background_color;
    uint8_t text_color;
    char *text;
    void (*onClick)(struct button *);
};
typedef struct button button_t;

bool (is_cursor_over_button)(button_t button, position_t mouse_position);
void (change_button_colors)(button_t* button, uint8_t new_background_color, uint8_t new_text_color);
int (draw_button)(button_t button);
int (draw_buttons)(button_t* buttons, int size);
