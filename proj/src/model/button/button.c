#include "button.h"

bool (is_cursor_over_button)(button_t *button, position_t mouse_position){
  bool is_between_x = mouse_position.x >= button->x && mouse_position.x <= button->x + button->width;
  bool is_between_y = mouse_position.y >= button->y && mouse_position.y <= button->y + button->height;
  return is_between_x && is_between_y;
}

void (change_button_color)(button_t* button, uint32_t new_background_color){
  button->background_color = new_background_color;
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
    buttons_array->buttons = malloc(sizeof(button_t*) * num_buttons);
    if (buttons_array->buttons == NULL) {
        free(buttons_array);
        return NULL;
    }
    buttons_array->num_buttons = num_buttons;
    return buttons_array;
}
void (destroy_buttons_array)(buttons_array_t *buttons_array) {
    for (int i = 0; i < buttons_array->num_buttons; i++) {
        destroy_button(buttons_array->buttons[i]);
    }
    free(buttons_array->buttons);
    free(buttons_array);
}

void (destroy_button)(button_t *button) {
  free(button->text);
  free(button);
}

button_t *(create_button)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t background_color, char *text, button_icon_t icon, void (*onClick)(struct button *)) {
  printf("creating button\n");
  button_t *button = malloc(sizeof(button_t));
  if (button == NULL) {
    return NULL;
  }
  button->x = x;
  button->y = y;
  button->width = width;
  button->height = height;
  button->background_color = background_color;
  if (text != NULL) {
    button->text = malloc(sizeof(char) * (strlen(text) + 1));
    if (button->text == NULL) {
      free(button);
      return NULL;
    }
    if (strcpy(button->text, text) == NULL) {
      free(button->text);
      free(button);
      printf("Error copying text to button inside %s\n", __func__);
      return NULL;
    }
    printf("button text: %s\n", button->text);
  }else{
    button->text = NULL;
  }

  button->icon = icon;
  button->onClick = onClick;
  printf("finished creating button\n");
  return button;
}
