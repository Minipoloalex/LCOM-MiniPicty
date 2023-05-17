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

int draw_button(button_t button) {
  if(vg_draw_rectangle(button.x, button.y, button.width, button.height, button.background_color)) return EXIT_FAILURE;
  // draw text inside button
  return EXIT_SUCCESS;
}

int draw_buttons(button_t* buttons, int size) {
  for(int i = 0; i < size; i++){
    //printf("drawing button %d\n", i);
    if(draw_button(buttons[i])) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

