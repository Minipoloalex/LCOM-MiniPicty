#include "game.h"

PlayerDrawer_t *player_drawer;
extern vbe_mode_info_t vmi;

button_t color_buttons[5];
button_t other_buttons[4];

//function that change the color of the brush
void change_brush_color(button_t* button){
  brush_t *brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  brush->color = button->background_color;
}

int (setup_game)() {
  // TODO: change this to depend on isTransmitter: the transmitter always starts the game drawing
  player_drawer = create_player_drawer(SELF_PLAYER);
  if (player_drawer == NULL) {
    printf("create_player_drawer inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  int min_len = vmi.XResolution / 9;
  int min_height = vmi.YResolution / 11;

  button_t red_button = {1, 1, min_len, min_height, 4, 0, "", NULL};
  button_t green_button = {2*min_len, 1, min_len, min_height, 2, 0, "", NULL};
  button_t blue_button = {4*min_len, 1, min_len, min_height, 9, 0, "", NULL};
  button_t yellow_button = {6*min_len, 1, min_len, min_height, 62, 0, "", NULL};
  button_t black_button = {8*min_len, 1, min_len, min_height, 0, 0, "", NULL};

  //red_button.onClick = change_brush_color;
  //green_button.onClick = change_brush_color;
  //blue_button.onClick = change_brush_color;
  //yellow_button.onClick = change_brush_color;
  //black_button.onClick = change_brush_color;

  color_buttons[0] = red_button;
  color_buttons[1] = green_button;
  color_buttons[2] = blue_button;
  color_buttons[3] = yellow_button;
  color_buttons[4] = black_button;

  int other_buttons_color = 56;

  button_t increase_size_button = {8*min_len, 3*min_height, min_len, min_height, other_buttons_color, 0, "Increase size", NULL};

  button_t decrease_size_button = {8*min_len, 5*min_height, min_len, min_height, other_buttons_color, 0, "Decrease size", NULL};

  button_t rubber_button = {8*min_len, 7*min_height, min_len, min_height, other_buttons_color, 0, "Rubber", NULL};

  button_t clear_button = {8*min_len, 9*min_height, min_len, min_height, other_buttons_color, 0, "Clear", NULL};

  other_buttons[0] = increase_size_button;
  other_buttons[1] = decrease_size_button;
  other_buttons[2] = rubber_button;
  other_buttons[3] = clear_button;

  return EXIT_SUCCESS;
}

void (destroy_game)() {
  destroy_player_drawer(player_drawer);
  // clear screen? double/triple buffering
}

int (game_process_mouse)() {
  player_t *player = player_drawer_get_player(player_drawer);
  drawing_position_t next = mouse_get_drawing_position_from_packet(player_get_current_position(player));
  if (player_drawer_get_state(player_drawer) == SELF_PLAYER) {  
    // if I'm the drawer, I must communicate what I'm doing
    ser_add_position_to_transmitter_queue(next);
    return player_add_next_position(player, &next);
  }
  return EXIT_SUCCESS;
}


int (game_process_serial)() {
  if (player_drawer_get_state(player_drawer) == OTHER_PLAYER) {
    ser_read_bytes_from_receiver_queue(player_drawer);
  }
  return EXIT_SUCCESS;
}

int (draw_game)(){
  if (draw_buttons(color_buttons, 5) != OK) return EXIT_FAILURE;
  if (draw_buttons(other_buttons, 4) != OK) return EXIT_FAILURE;
  if (vg_draw_player_drawer(player_drawer) != OK) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
