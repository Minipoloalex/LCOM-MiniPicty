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

int (get_hovered_button)(button_t *buttons, int num_buttons, position_t mouse_position) {
  for (int i = 0; i < num_buttons; i++) {
    if (is_cursor_over_button(buttons[i], mouse_position)) {
      return i;
    }
  }
  return -1;
}

int (process_buttons_clicks)(button_t *buttons, int num_buttons, drawing_position_t before, drawing_position_t next, int *button_clicked) {
  static int pressed_button = -1;
  static int button_hovered = -1;
  printf("values of - pressed_button: %d, button_hovered: %d\n", pressed_button, button_hovered);
  button_hovered = get_hovered_button(buttons, num_buttons, next.position);
  if (pressed_button != -1 && pressed_button == button_hovered
  && !next.is_drawing && before.is_drawing) {
    printf("clicking on button %d\n", pressed_button);
    *button_clicked = pressed_button;
    pressed_button = -1;
    return EXIT_SUCCESS;
  }
  else if (button_hovered != -1 && next.is_drawing && !before.is_drawing) {
    printf("pressed button %d\n", button_hovered);
    pressed_button = button_hovered;
  }
  else if (before.is_drawing && !next.is_drawing && pressed_button != -1 
  && button_hovered != pressed_button) {
    printf("releasing button %d\n", pressed_button);
    pressed_button = -1;
  }
  return EXIT_SUCCESS;
}
