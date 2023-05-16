#include "game.h"

#define NUMBER_GAME_BUTTONS 14
button_t game_buttons[NUMBER_GAME_BUTTONS];

PlayerDrawer_t *player_drawer;
extern vbe_mode_info_t vmi;

//function that change the color of the brush
void change_brush_color(button_t* button){
  printf("Detected click on button %d\n", button->background_color);
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

  button_t red_button = {0, 0, min_len, min_height, 4, 0, "", NULL};
  button_t green_button = {min_len, 0, min_len, min_height, 2, 0, "", NULL};
  button_t blue_button = {2*min_len, 0, min_len, min_height, 9, 0, "", NULL};
  button_t yellow_button = {3*min_len, 0, min_len, min_height, 62, 0, "", NULL};
  button_t black_button = {4*min_len, 0, min_len, min_height, 0, 0, "", NULL};
  button_t gray_button = {5*min_len, 0, min_len, min_height, 56, 0, "", NULL};
  button_t orange_button = {6*min_len, 0, min_len, min_height, 38, 0, "", NULL};
  button_t purple_button = {7*min_len, 0, min_len, min_height, 33, 0, "", NULL};
  button_t pink_button = {8*min_len, 0, min_len, min_height, 53, 0, "", NULL};

  red_button.onClick = change_brush_color;
  green_button.onClick = change_brush_color;
  blue_button.onClick = change_brush_color;
  yellow_button.onClick = change_brush_color;
  black_button.onClick = change_brush_color;

  game_buttons[0] = red_button;
  game_buttons[1] = green_button;
  game_buttons[2] = blue_button;
  game_buttons[3] = yellow_button;
  game_buttons[4] = black_button;
  game_buttons[5] = gray_button;
  game_buttons[6] = orange_button;
  game_buttons[7] = purple_button;
  game_buttons[8] = pink_button;


  int other_buttons_color = 56;

  button_t increase_size_button = {8*min_len, 2*min_height, min_len, min_height, other_buttons_color, 0, "Increase size", NULL};

  button_t decrease_size_button = {8*min_len, 4*min_height, min_len, min_height, other_buttons_color, 0, "Decrease size", NULL};

  button_t rubber_button = {8*min_len, 6*min_height, min_len, min_height, other_buttons_color, 0, "Rubber", NULL};

  button_t clear_button = {8*min_len, 8*min_height, min_len, min_height, other_buttons_color, 0, "Clear", NULL};

  button_t canvas = {0, min_height, 8*min_len, 8*min_height, 63, 0, "", NULL};

  game_buttons[9] = increase_size_button;
  game_buttons[10] = decrease_size_button;
  game_buttons[11] = rubber_button;
  game_buttons[12] = clear_button;
  game_buttons[13] = canvas;

  red_button.onClick(&red_button);
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
  if (draw_buttons(game_buttons, NUMBER_GAME_BUTTONS) != OK) return EXIT_FAILURE;
  if (vg_draw_player_drawer(player_drawer) != OK) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int (is_cursor_over_game_button)(position_t mouse_position){
  for(int i = 0; i < NUMBER_GAME_BUTTONS; i++){
    if(is_cursor_over_button(game_buttons[i], mouse_position)){
      return i;
    }
  }
  return -1;
}

// ABOUT GAME_PROCESS_MOUSE():
// once we receive a mouse release signal, we check if any button is hovered
// is_cursor_over_game_button returns the index of the button if it is hovered, -1 otherwise
// button_index = is_cursor_over_game_button(mouse_position)
// then we execute game_buttons[button_index].onClick(&game_buttons[button_index])

// if it is not possible to define a function pointer for the onClick function, we can use a switch case

// switch(button_index){
//   case 0:
//   case 1:
//   case 2:
//   ...
//      change_brush_color(&game_buttons[button_index]);
//      break;
//   case 9:
