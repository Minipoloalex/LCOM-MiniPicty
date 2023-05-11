#include "menu.h"

uint16_t menu_mode = GRAPHICS_MODE_0;
extern vbe_mode_info_t vmi;

player_menu_t *player_menu;
struct button menu_buttons[3];


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

  struct button play_button = {x, height, width, height, background_color, text_color, "PLAY"};
  struct button settings_button = {x, height * 3, width, height, background_color, text_color, "LEADERBOARD"};
  struct button exit_button = {x, height * 5, width, height, background_color, text_color, "EXIT"};

  menu_buttons[0] = play_button;
  menu_buttons[1] = settings_button;
  menu_buttons[2] = exit_button;
}

int (draw_player_menu)() {
  player_t *player = player_menu_get_player(player_menu);
  drawing_position_t drawing_position;
  position_t last_position;

  while (player_get_next_position(player, &drawing_position) == OK) {
    if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;
    set_needs_update(true);
    if (drawing_position.is_drawing) {
      // TODO: check for collisions with buttons
    }
    player_set_last_position(player, drawing_position.position);
  }
  return EXIT_SUCCESS;
}

int (draw_buttons)() {
  position_t last_position;
  player_t *player = player_menu_get_player(player_menu);
  if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;

  for(int i = 0; i < 3; i++){
    draw_button(&menu_buttons[i], &last_position);
  }
  return 0;
}

void (draw_menu)(){
  if (draw_player_menu()) {
    printf("draw_player_menu inside %s\n", __func__);
    return;
  }
  if (buffers_need_update()) {  // if no new mouse positions, don't update anything
    if(draw_buttons()) {
      printf("Error drawing buttons\n");
      return;
    }
    if (vg_buffer_flip()) {
      printf("vg_buffer_flip inside %s\n", __func__);
    }
  }
}

int (menu_process_mouse)() {
  player_t *player = player_menu_get_player(player_menu);
  drawing_position_t next = mouse_get_drawing_position_from_packet(player_get_current_position(player));
  return player_add_next_position(player, &next);
}

void (destroy_menu)() {
  destroy_player_menu(player_menu);
  vg_clear_buffers();
}
