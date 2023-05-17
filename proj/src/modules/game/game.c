#include "game.h"

#define NUMBER_GAME_BUTTONS 13
button_t game_buttons[NUMBER_GAME_BUTTONS];

static player_drawer_t *player_drawer;
static canvas_t *canvas;
extern vbe_mode_info_t vmi;

void change_brush_color(button_t* button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  brush_set_color(brush, button->background_color);
}

void increase_brush_size(button_t* button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  brush_increase_size(brush);
}

void decrease_brush_size(button_t* button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  brush_decrease_size(brush);
}

void set_rubber(button_t* button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  brush_set_color(brush, canvas->background_color);
}

void clear_canvas(button_t* button){
  if (canvas == NULL) return;
  canvas_clear(canvas);
}

int (setup_game)(bool isTransmitter) {
  player_drawer = create_player_drawer(isTransmitter ? SELF_PLAYER : OTHER_PLAYER);
  if (player_drawer == NULL) {
    printf("create_player_drawer inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  int min_len = vmi.XResolution / 9;
  int min_height = vmi.YResolution / 11;

  button_t red_button = {0, 0, min_len, min_height, 4, 0, "", change_brush_color};
  button_t green_button = {min_len, 0, min_len, min_height, 2, 0, "", change_brush_color};
  button_t blue_button = {2*min_len, 0, min_len, min_height, 9, 0, "", change_brush_color};
  button_t yellow_button = {3*min_len, 0, min_len, min_height, 62, 0, "", change_brush_color};
  button_t black_button = {4*min_len, 0, min_len, min_height, 0, 0, "", change_brush_color};
  button_t gray_button = {5*min_len, 0, min_len, min_height, 56, 0, "", change_brush_color};
  button_t orange_button = {6*min_len, 0, min_len, min_height, 38, 0, "", change_brush_color};
  button_t purple_button = {7*min_len, 0, min_len, min_height, 33, 0, "", change_brush_color};
  button_t pink_button = {8*min_len, 0, min_len, min_height, 53, 0, "", change_brush_color};

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

  button_t increase_size_button = {8*min_len, 2*min_height, min_len, min_height, other_buttons_color, 0, "Increase size", increase_brush_size};

  button_t decrease_size_button = {8*min_len, 4*min_height, min_len, min_height, other_buttons_color, 0, "Decrease size", decrease_brush_size};

  button_t rubber_button = {8*min_len, 6*min_height, min_len, min_height, other_buttons_color, 0, "Rubber", set_rubber};

  button_t clear_button = {8*min_len, 8*min_height, min_len, min_height, other_buttons_color, 0, "Clear", clear_canvas};

  game_buttons[9] = increase_size_button;
  game_buttons[10] = decrease_size_button;
  game_buttons[11] = rubber_button;
  game_buttons[12] = clear_button;

  canvas = canvas_init(0, min_height, 8*min_len, 8*min_height);
  if (canvas == NULL) {
    destroy_player_drawer(player_drawer);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (destroy_game)() {
  destroy_player_drawer(player_drawer);
  canvas_destroy(canvas);
  vg_clear_buffers();
}

int (game_process_mouse)() {
  player_t *player = player_drawer_get_player(player_drawer);
  drawing_position_t next = mouse_get_drawing_position_from_packet(player_get_current_position(player));

  int button_index = is_cursor_over_game_button(next.position);
  if (button_index != -1) {
    game_buttons[button_index].onClick(&game_buttons[button_index]);
  }

  if (player_drawer_get_state(player_drawer) == SELF_PLAYER) {  
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
  if (draw_to_canvas(canvas, player_drawer) != OK) {
    printf("draw_to_canvas inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (buffers_need_update()) {
    // copy do canvas para o buffer
    if (vg_copy_canvas_buffer(get_buffer(canvas))) {
      printf("vg_copy_canvas_buffer inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    // draw buttons
    if (draw_buttons(game_buttons, NUMBER_GAME_BUTTONS) != OK) return EXIT_FAILURE;
    if (vg_buffer_flip()) {
      printf("vg_buffer_flip inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
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
