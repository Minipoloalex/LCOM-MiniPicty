#include "game.h"

#define NUMBER_GAME_BUTTONS 13
button_t game_buttons[NUMBER_GAME_BUTTONS];

extern vbe_mode_info_t vmi;

static player_drawer_t *player_drawer;
static canvas_t *canvas;
static guess_word_t *guess;
static char prompt[15];

/*==================================================================*/
/* SHOULD THESE BE BUTTON FUNCTIONS ? */
void change_brush_color(button_t* button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  set_brush_color(brush, button->background_color);
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
  set_brush_color(brush, canvas->background_color);
}

void clear_canvas(button_t* button){
  if (canvas == NULL) return;
  canvas_clear(canvas);
}
/*==================================================================*/
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

  button_t increase_size_button = {8*min_len, 2*min_height, min_len, min_height, other_buttons_color, 0, "IncreaseSize", increase_brush_size};

  button_t decrease_size_button = {8*min_len, 4*min_height, min_len, min_height, other_buttons_color, 0, "DecreaseSize", decrease_brush_size};

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
  guess = create_guess_word();
  if (guess == NULL) {
    destroy_player_drawer(player_drawer);
    canvas_destroy(canvas);
    return EXIT_FAILURE;
  }
  if (prompt_generate(prompt) != 0){
    destroy_player_drawer(player_drawer);
    canvas_destroy(canvas);
    destroy_guess_word(guess);
  }
  return EXIT_SUCCESS;
}

void (destroy_game)() {
  destroy_player_drawer(player_drawer);
  canvas_destroy(canvas);
  destroy_guess_word(guess);
  vg_clear_buffers();
}

extern int keyboard_return_value;
extern uint8_t scancode;
int (game_process_keyboard)(){
  if (player_drawer_get_state(player_drawer) == SELF_PLAYER) return EXIT_SUCCESS;
  if (keyboard_return_value) return EXIT_SUCCESS;
  if (scancode == 0) return EXIT_SUCCESS;

  //printf("trying to process kbd\n");
  if (scancode == MAKE_BACKSPACE){
    //printf("apagando\n");
    //printf("old size: %d", guess->pointer);
    delete_character(guess);
    //printf("new size: %d\n", guess->pointer);
  }
  else if (scancode == MAKE_ENTER){
    bool right;
    //printf("validando\n");
    validate_guess_word(prompt, guess, &right);
    printf("correct guess: %d \n", right);
    reset_guess_word(guess);

    //TODO se acertar, avançar
  }
  else {
    bool is_break = false;
    if (is_breakcode(scancode, &is_break)) return EXIT_FAILURE;

    if (!is_break){
      uint8_t caracter = 0;
      if (translate_scancode(scancode, &caracter)) return EXIT_FAILURE;
      if (caracter != 0){
        //printf("writing\n");
        //printf("old size: %d ", guess->pointer);
        if (write_character(guess, caracter)) return EXIT_FAILURE;
        //printf("new size: %d\n", guess->pointer);
      }
    }
  }
  //printf("processed kbd\n");
  return EXIT_SUCCESS;
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
  if (/*buffers_need_update()*/ true) {
    // copy do canvas para o buffer
    if (vg_copy_canvas_buffer(get_buffer(canvas))) {
      printf("vg_copy_canvas_buffer inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    //printf("%d\n", guess->pointer);
    if (vg_draw_rectangle(GUESS_BOX_X,GUESS_BOX_Y, GUESS_BOX_WIDTH, GUESS_BOX_HEIGHT, BLACK)) return EXIT_FAILURE;
    switch (player_drawer_get_state(player_drawer)){
      case SELF_PLAYER:
        if (vg_draw_text(prompt, GUESS_POS_X, GUESS_POS_Y) != OK) return EXIT_FAILURE;
        break;
      case OTHER_PLAYER:
        if (vg_draw_guess(guess, GUESS_POS_X, GUESS_POS_Y) != OK) return EXIT_FAILURE;
        break;
    }
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
