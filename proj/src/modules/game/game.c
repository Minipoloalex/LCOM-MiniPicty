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
  if (canvas == NULL) {
    printf("canvas is null inside %s\n", __func__);
    return;
  }
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

  button_t red_button = {0, 0, min_len, min_height, 0XFF0000, "", change_brush_color};
  button_t green_button = {min_len, 0, min_len, min_height, 0X00FF00, "", change_brush_color};
  button_t blue_button = {2*min_len, 0, min_len, min_height, 0X0066CC, "", change_brush_color};
  button_t yellow_button = {3*min_len, 0, min_len, min_height, 0XFFFF00, "", change_brush_color};
  button_t black_button = {4*min_len, 0, min_len, min_height, 0X000000, "", change_brush_color};
  button_t gray_button = {5*min_len, 0, min_len, min_height, 0XA0A0A0, "", change_brush_color};
  button_t orange_button = {6*min_len, 0, min_len, min_height, 0XFF9933, "", change_brush_color};
  button_t purple_button = {7*min_len, 0, min_len, min_height, 0X660066, "", change_brush_color};
  button_t pink_button = {8*min_len, 0, min_len, min_height, 0XFF99FF, "", change_brush_color};

  game_buttons[0] = red_button;
  game_buttons[1] = green_button;
  game_buttons[2] = blue_button;
  game_buttons[3] = yellow_button;
  game_buttons[4] = black_button;
  game_buttons[5] = gray_button;
  game_buttons[6] = orange_button;
  game_buttons[7] = purple_button;
  game_buttons[8] = pink_button;

  int other_buttons_color = 0XA0A0A0;

  button_t increase_size_button = {8*min_len, 2*min_height, min_len, min_height, other_buttons_color, "Increase", increase_brush_size};

  button_t decrease_size_button = {8*min_len, 4*min_height, min_len, min_height, other_buttons_color, "Decrease", decrease_brush_size};

  button_t rubber_button = {8*min_len, 6*min_height, min_len, min_height, other_buttons_color,"Rubber", set_rubber};

  button_t clear_button = {8*min_len, 8*min_height, min_len, min_height, other_buttons_color, "Clear", clear_canvas};

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

void (transitionToGame)(state_t* state){
  defaultImplementation(state);
  state->draw = game_draw;
  state->process_mouse = game_process_mouse;
  state->process_keyboard = game_process_keyboard;
  state->process_serial = game_process_serial;
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
  drawing_position_t before = player_get_current_position(player);
  drawing_position_t next = mouse_get_drawing_position_from_packet(before.position);
  if (player_drawer_get_state(player_drawer) == SELF_PLAYER) {  
    ser_add_position_to_transmitter_queue(next);
    if (player_add_next_position(player, &next) != 0) {
      printf("player_add_next_position inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  int button_to_click = -1;
  if (process_buttons_clicks(game_buttons, NUMBER_GAME_BUTTONS, before, next, &button_to_click)) {
    printf("process_buttons_clicks inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (button_to_click != -1) {
    button_t pressed_button = game_buttons[button_to_click];
    ser_add_button_click_to_transmitter_queue(button_to_click);
    pressed_button.onClick(&pressed_button);
  }
  return EXIT_SUCCESS;
}


int (game_process_serial)() {
  if (player_drawer_get_state(player_drawer) == OTHER_PLAYER) {
    ser_read_bytes_from_receiver_queue(player_drawer, game_buttons, NUMBER_GAME_BUTTONS);
  }
  return EXIT_SUCCESS;
}

int (game_draw)(){
  if (draw_to_canvas(canvas, player_drawer) != OK) {
    printf("draw_to_canvas inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (/*buffers_need_update()*/ true) {
    if (vg_copy_canvas_buffer(get_buffer(canvas)) != OK) {
      printf("vg_copy_canvas_buffer inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    //printf("%d\n", guess->pointer);
    if (vg_draw_buttons(game_buttons, NUMBER_GAME_BUTTONS)) {
      printf("vg_draw_buttons inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (vg_draw_rectangle(GUESS_BOX_X,GUESS_BOX_Y, GUESS_BOX_WIDTH, GUESS_BOX_HEIGHT, BLACK)) return EXIT_FAILURE;
    switch (player_drawer_get_state(player_drawer)){
      case SELF_PLAYER:
        if (vg_draw_text(prompt, GUESS_POS_X, GUESS_POS_Y) != OK) return EXIT_FAILURE;
        break;
      case OTHER_PLAYER:
        if (vg_draw_guess(guess, GUESS_POS_X, GUESS_POS_Y) != OK) return EXIT_FAILURE;
        break;
    }
    
    player_t *player = player_drawer_get_player(player_drawer);
    drawing_position_t drawing_position = player_get_current_position(player);
    update_cursor_state(drawing_position.position);
    cursor_image_t cursor = player_drawer_get_cursor(player_drawer);

    if (vg_draw_cursor(cursor, drawing_position.position) != OK){
      printf("vg_draw_cursor inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (vg_buffer_flip()) {
      printf("vg_buffer_flip inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

// int (is_cursor_over_game_button)(position_t mouse_position){
//   for(int i = 0; i < NUMBER_GAME_BUTTONS; i++){
//     if(is_cursor_over_button(game_buttons[i], mouse_position)){
//       player_drawer_set_cursor(player_drawer, POINTER);
//       return i;
//     }
//   }
//   player_drawer_set_cursor(player_drawer, PEN);
//   return -1;
// }

void (update_cursor_state)(position_t position){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (!canvas_contains_position(canvas, position)){
    player_drawer_set_cursor(player_drawer, POINTER);
  }
  else if(brush->color == canvas->background_color){
    player_drawer_set_cursor(player_drawer, RUBBER);
  }
  else {
    player_drawer_set_cursor(player_drawer, PEN);
  }
} 

