#pragma once

#include <lcom/lcf.h>
#include "../position/position.h"
#include "../drawing_position.h"
#include "../../resources/icons.h"

typedef enum {
    PLUS,
    MINUS,
    RUBBER_ICON,
    TRASH,
    NOTHING,
} button_icon_t;

struct button {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint32_t background_color;
    char *text;
    button_icon_t icon;
    void (*onClick)(struct button *);
};
typedef struct button button_t;

typedef struct buttons_array {
    button_t *buttons;
    int num_buttons;
} buttons_array_t;


bool (is_cursor_over_button)(button_t button, position_t mouse_position);
int (get_hovered_button)(buttons_array_t *buttons_array, position_t mouse_position);
void (change_button_color)(button_t* button, uint32_t new_background_color);

int (process_buttons_clicks)(buttons_array_t *buttons_array, drawing_position_t before, drawing_position_t next, int *button_clicked);

buttons_array_t *(create_buttons_array)(int num_buttons);

void (destroy_buttons_array)(buttons_array_t *buttons_array);
