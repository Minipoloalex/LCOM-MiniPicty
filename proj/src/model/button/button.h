#pragma once

#include <lcom/lcf.h>
#include "../position/position.h"
#include "../../modules/resources/icons.h"

/** @defgroup button Button
 * @{
 *
 * @brief Module responsible for the button
 */

/**
 * @brief Button icon enum
 * Specifies the icon that the button will have
 */
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
 * Can have also an icon or not, and/or text.
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
 * @brief Creates a button with the specified parameters
 * 
 * @return button_t* pointer to the button
 */
button_t *(create_button)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t background_color, char *text, button_icon_t icon, void (*onClick)(struct button *));
/**
 * @brief Destroys a button by freeing its memory
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
 * 
 * @param num_buttons number of buttons in the array
 * @return buttons_array_t* pointer to the array of buttons
 */
buttons_array_t *(create_buttons_array)(int num_buttons);
/**
 * @brief Destroys an array of buttons by freeing its memory
 * 
 * @param buttons_array pointer to the array of buttons
 */
void (destroy_buttons_array)(buttons_array_t *buttons_array);

/**
 * @brief Checks if the mouse position is inside the button
 * This is used for changing its color and knowing if it was clicked or not.
 * 
 * @param button button to check
 * @param mouse_position mouse's position
 */
bool (is_cursor_over_button)(button_t *button, position_t mouse_position);

/**
 * @brief Gets the hovered button's index
 * 
 * @param buttons_array array of buttons to check
 * @param mouse_position mouse's position
 * 
 * @return -1 if no button is hovered, the button's index otherwise
 */
int (get_hovered_button)(buttons_array_t *buttons_array, position_t mouse_position);
/**
 * @brief Changes a button's color
 * 
 * @param button button to change it's color
 * @param new_background_color new color
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
