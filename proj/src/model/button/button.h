#pragma once

#include <lcom/lcf.h>
#include "../position/position.h"
#include "../../modules/resources/icons.h"

typedef enum {
    PLUS_ICON,
    MINUS_ICON,
    RUBBER_ICON,
    TRASH_ICON,
    NO_ICON,
} button_icon_t;

/**
 * @brief Button struct.
 * Has a start x and y position, width, height and a color.
 * Can have also an icon or not.
 * Its also responsible for holding a reference to the on click callback.
 */
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
/**
 * @brief Creates a button
 */
button_t *(create_button)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t background_color, char *text, button_icon_t icon, void (*onClick)(struct button *));
/**
 * @brief Destroys a button
 */
void (destroy_button)(button_t *button);

/**
 * @brief Buttons array struct.
 * Has an array of buttons and the corresponded number of buttons in the array.
 */
typedef struct buttons_array {
    button_t **buttons;
    int num_buttons;
} buttons_array_t;
/**
 * @brief Creates an array of buttons
 */
buttons_array_t *(create_buttons_array)(int num_buttons);
/**
 * @brief Destroys an array of buttons
 */
void (destroy_buttons_array)(buttons_array_t *buttons_array);

/**
 * @brief Checks if the mouse position in inside the button
 * This is used for changing its color and
 */
bool (is_cursor_over_button)(button_t *button, position_t mouse_position);

/**
 * @brief Return the index of the hovered button, -1 if none is beeing hovered.
 */
int (get_hovered_button)(buttons_array_t *buttons_array, position_t mouse_position);
/**
 * @brief Changes the button color
 */
void (change_button_color)(button_t* button, uint32_t new_background_color);

/**
 * @brief Processes a button click
 * Know if a button was clicked or not and which one was clicked.
 * A state machine is used to know if the mouse was pressed and released inside the button.
 * 
 * @param buttons_array array of buttons
 * @param before position before the click
 * @param next position after the click
 * @param button_clicked pointer to the button clicked
 * @return 0 if no button was clicked, 1 if a button was clicked
 */
int (process_buttons_clicks)(buttons_array_t *buttons_array, drawing_position_t before, drawing_position_t next, int *button_clicked);
