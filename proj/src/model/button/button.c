#include "button.h"

bool (is_cursor_over_button)(button_t button, position_t mouse_position){
  bool is_between_x = mouse_position.x >= button.x && mouse_position.x <= button.x + button.width;
  bool is_between_y = mouse_position.y >= button.y && mouse_position.y <= button.y + button.height;
  return is_between_x && is_between_y;
}

void (change_button_colors)(button_t* button, uint8_t new_background_color, uint8_t new_text_color){
  button->background_color = new_background_color;
  button->text_color = new_text_color;
}

int (get_hovered_button)(buttons_array_t *buttons_array, position_t mouse_position) {
  int num_buttons = buttons_array->num_buttons;
  for (int i = 0; i < num_buttons; i++) {
    if (is_cursor_over_button(buttons_array->buttons[i], mouse_position)) {
      return i;
    }
  }
  return -1;
}

int (process_buttons_clicks)(buttons_array_t *buttons_array, drawing_position_t before, drawing_position_t next, int *button_clicked) {
  static int pressed_button = -1;
  static int button_hovered = -1;
  button_hovered = get_hovered_button(buttons_array, next.position);
  if (pressed_button != -1 && pressed_button == button_hovered
&& !next.is_drawing && before.is_drawing) {
    *button_clicked = pressed_button;
    pressed_button = -1;
    return EXIT_SUCCESS;
  }
  else if (button_hovered != -1 && next.is_drawing && !before.is_drawing) {
    pressed_button = button_hovered;
  }
  else if (before.is_drawing && !next.is_drawing && pressed_button != -1 
  && button_hovered != pressed_button) {
    pressed_button = -1;
  }
  return EXIT_SUCCESS;
}

buttons_array_t *(create_buttons_array)(int num_buttons) {
    buttons_array_t *buttons_array = malloc(sizeof(buttons_array_t));
    if (buttons_array == NULL) {
        return NULL;
    }
    buttons_array->buttons = malloc(sizeof(button_t) * num_buttons);
    if (buttons_array->buttons == NULL) {
        free(buttons_array);
        return NULL;
    }
    buttons_array->num_buttons = num_buttons;
    printf("num_buttons: %d\n", num_buttons);
    return buttons_array;
}
void (destroy_buttons_array)(buttons_array_t *buttons_array) {
    free(buttons_array->buttons);
    free(buttons_array);
}
