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
