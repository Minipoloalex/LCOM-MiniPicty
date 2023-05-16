#include "menu.h"
#define NUMBER_MENU_BUTTONS 3
button_t menu_buttons[NUMBER_MENU_BUTTONS];

uint16_t menu_mode = GRAPHICS_MODE_0;
extern vbe_mode_info_t vmi;

player_menu_t *player_menu;


void (setup_menu)() {
  player_menu = create_player_menu();
  if (player_menu == NULL) {
    printf("create_player_menu inside %s\n", __func__);
    return;
  }
  uint16_t x = vmi.XResolution / 3;
  uint16_t width = vmi.XResolution / 3;
  uint16_t height = vmi.YResolution / 7;

  uint8_t background_color = 10;  
  uint8_t text_color = 5;

  button_t play_button = {x, height, width, height, background_color, text_color, "PLAY", NULL};
  button_t settings_button = {x, height * 3, width, height, background_color, text_color, "LEADERBOARD", NULL};
  button_t exit_button = {x, height * 5, width, height, background_color, text_color, "EXIT", NULL};

  menu_buttons[0] = play_button;
  menu_buttons[1] = settings_button;
  menu_buttons[2] = exit_button;
}

int (draw_menu_buttons)(){
  player_t *player = player_menu_get_player(player_menu);
  drawing_position_t drawing_position;
  position_t last_position;

  if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;
  while (player_get_next_position(player, &drawing_position) == OK) {
    if (drawing_position.is_drawing) {
      // TODO: check for collisions with buttons
    }
    printf("position: x %d y %d pressed %d\n", drawing_position.position.x, drawing_position.position.y, drawing_position.is_drawing);
    player_set_last_position(player, drawing_position.position);
    player_get_last_position(player, &last_position);
  }

  position_t position = player_get_current_position(player);
  for(int i = 0; i < NUMBER_MENU_BUTTONS; i++){
    if(is_cursor_over_button(menu_buttons[i], position)){
      change_button_colors(&menu_buttons[i], 10, 5);
    } else {
      change_button_colors(&menu_buttons[i], 5, 10);
    }
  }

  return draw_buttons(menu_buttons, NUMBER_MENU_BUTTONS);
}

int (draw_menu)(){
  if(draw_menu_buttons()){
    printf("Error drawing buttons\n");
    return 1;
  }
  return 0;
}

int (menu_process_mouse)() {
  player_t *player = player_menu_get_player(player_menu);
  drawing_position_t next = mouse_get_drawing_position_from_packet(player_get_current_position(player));
  return player_add_next_position(player, &next);
}

void (destroy_menu)() {
  destroy_player_menu(player_menu);
  // clear screen? double/triple buffering
}
