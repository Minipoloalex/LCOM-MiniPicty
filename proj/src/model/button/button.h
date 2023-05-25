#pragma once

#include <lcom/lcf.h>
#include "../position/position.h"
#include "../drawing_position.h"

struct button {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    int background_color;
    int text_color;
    char *text;
    void (*onClick)(struct button *);
};
typedef struct button button_t;

bool (is_cursor_over_button)(button_t button, position_t mouse_position);
int (get_hovered_button)(button_t *buttons, int num_buttons, position_t mouse_position);
void (change_button_colors)(button_t* button, int new_background_color, int new_text_color);

int (process_buttons_clicks)(button_t *button, int num_buttons, drawing_position_t before, drawing_position_t next, int *button_clicked);

