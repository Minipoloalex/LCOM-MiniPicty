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
};

bool (is_button_hovered)(struct button* button, position_t* mouse_position);
void (change_button_colors)(struct button* button, uint8_t new_background_color, uint8_t new_text_color);
void (draw_button)(struct button* button);
