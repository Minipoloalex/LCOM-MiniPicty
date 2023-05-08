#include "button.h"

bool (is_button_hovered)(struct button* button, position_t* mouse_position){
  bool is_between_x = mouse_position->x >= button->x && mouse_position->x <= button->x + button->width;
  bool is_between_y = mouse_position->y >= button->y && mouse_position->y <= button->y + button->height;
  return is_between_x && is_between_y;
}

void (change_button_colors)(struct button* button, uint8_t new_background_color, uint8_t new_text_color){
  button->background_color = new_background_color;
  button->text_color = new_text_color;
}

void draw_button(struct button* button, position_t *mouse_position) {
  if(is_button_hovered(button, mouse_position)){
    change_button_colors(button, 10, 5);
  } else {
    change_button_colors(button, 5, 10);
  }
  vg_draw_rectangle(button->x, button->y, button->width, button->height, button->background_color);
  // draw text inside button
}
