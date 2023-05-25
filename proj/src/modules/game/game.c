#include "game.h"

#define NUMBER_GAME_PLAYING_BUTTONS 13
#define NUMBER_GAME_FINISHED_BUTTONS 3

static buttons_array_t *game_playing_buttons;
static buttons_array_t *game_finished_buttons;

extern vbe_mode_info_t vmi;

static player_drawer_t *player_drawer;
static canvas_t *canvas;
static guess_word_t *guess;
extern vbe_mode_info_t vmi;
static char prompt[15];
uint8_t round_timer;
typedef enum game_state {
  PLAYING,
  WAITING,
  FINISHED
} game_state_t;
static game_state_t game_state;
static state_t *app_state;
/*==================================================================*/
/**
 * @brief 
 * 
 */
int (game_draw_buttons)();
/*==================================================================*/ 
/* SHOULD THESE BE BUTTON FUNCTIONS ? */
void (play_again)(button_t *button) {
  if (game_state != FINISHED) {
    printf("Wrong game state inside %s\n", __func__);
    return;
  }
  game_state = WAITING;
  canvas_clear(canvas);
  if (player_drawer_get_role(player_drawer) == SELF_PLAYER) {
    if (prompt_generate(prompt) != OK) {  // communication of word index is not implemented yet
      printf("prompt_generate inside %s\n", __func__);
      return;
    }
  }
  reset_guess_word(guess);
}
void (quit_game)(button_t *button) {
  if (game_state != FINISHED) {
    printf("Wrong game state inside %s\n", __func__);
    return;
  }
  transition_to_menu(app_state);
}
void (play_again_change_roles)(button_t *button) {
  player_drawer_change_role(player_drawer);
  play_again(button);
}
void change_brush_color(button_t *button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  set_brush_color(brush, button->background_color);
}
void (increase_brush_size)(button_t* button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  brush_increase_size(brush);
}

void (decrease_brush_size)(button_t* button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  brush_decrease_size(brush);
}
void (set_rubber)(button_t* button){
  brush_t* brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) return;
  set_brush_color(brush, canvas->background_color);
}
void (clear_canvas)(button_t* button){
  if (canvas == NULL) {
    printf("canvas is null inside %s\n", __func__);
    return;
  }
  canvas_clear(canvas);
}
/*==================================================================*/
int (setup_game)(bool isTransmitter, state_t *state) {
  app_state = state;
  player_drawer = create_player_drawer(isTransmitter ? SELF_PLAYER : OTHER_PLAYER);
  if (player_drawer == NULL) {
    printf("create_player_drawer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  game_playing_buttons = create_buttons_array(NUMBER_GAME_PLAYING_BUTTONS);
  if (game_playing_buttons->buttons == NULL) {
    destroy_player_drawer(player_drawer);
    printf("create_buttons_array inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  game_finished_buttons = create_buttons_array(NUMBER_GAME_FINISHED_BUTTONS);
  if (game_finished_buttons->buttons == NULL) {
    destroy_player_drawer(player_drawer);
    destroy_buttons_array(game_playing_buttons);
    printf("create_buttons_array inside %s\n", __func__);
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

  game_playing_buttons->buttons[0] = red_button;
  game_playing_buttons->buttons[1] = green_button;
  game_playing_buttons->buttons[2] = blue_button;
  game_playing_buttons->buttons[3] = yellow_button;
  game_playing_buttons->buttons[4] = black_button;
  game_playing_buttons->buttons[5] = gray_button;
  game_playing_buttons->buttons[6] = orange_button;
  game_playing_buttons->buttons[7] = purple_button;
  game_playing_buttons->buttons[8] = pink_button;

  int other_buttons_color = 56;

  button_t increase_size_button = {8*min_len, 2*min_height, min_len, min_height, other_buttons_color, 0, "IncreaseSize", increase_brush_size};

  button_t decrease_size_button = {8*min_len, 4*min_height, min_len, min_height, other_buttons_color, 0, "DecreaseSize", decrease_brush_size};

  button_t rubber_button = {8*min_len, 6*min_height, min_len, min_height, other_buttons_color, 0, "Rubber", set_rubber};

  button_t clear_button = {8*min_len, 8*min_height, min_len, min_height, other_buttons_color, 0, "Clear", clear_canvas};

  game_playing_buttons->buttons[9] = increase_size_button;
  game_playing_buttons->buttons[10] = decrease_size_button;
  game_playing_buttons->buttons[11] = rubber_button;
  game_playing_buttons->buttons[12] = clear_button;


  button_t play_again_button = {8*min_len, 10*min_height, min_len, min_height, other_buttons_color, 0, "Send", play_again};
  button_t play_again_change_state = {8*min_len, 10*min_height, min_len, min_height, other_buttons_color, 0, "PlayAgain", play_again_change_roles};
  button_t quit_button = {8*min_len, 10*min_height, min_len, min_height, other_buttons_color, 0, "Quit", quit_game};
  
  game_finished_buttons->buttons[0] = play_again_button;
  game_finished_buttons->buttons[1] = play_again_change_state;
  game_finished_buttons->buttons[2] = quit_button;

  round_timer = ROUND_TIME;

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

void (transition_to_game)(state_t* state){
  default_implementation(state);
  state->draw = game_draw;
  state->process_mouse = game_process_mouse;
  state->process_keyboard = game_process_keyboard;
  state->process_serial = game_process_serial;
  state->process_timer = game_process_timer;
  state->get_buttons = get_game_buttons;
  
  prompt_generate(prompt);
  canvas_clear(canvas);
  game_state = PLAYING;
}

extern int timer_counter;
int (game_process_timer)(){
  if (timer_counter % sys_hz() == 0){
    round_timer--;
  }
  return EXIT_SUCCESS;
}

extern int keyboard_return_value;
extern uint8_t scancode;
int (game_process_keyboard)() {
  if (player_drawer_get_role(player_drawer) == SELF_PLAYER) return EXIT_SUCCESS;
  if (game_state != PLAYING) return EXIT_SUCCESS;

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
  if (player_drawer_get_role(player_drawer) == SELF_PLAYER) {  
    ser_add_position_to_transmitter_queue(next);
    if (player_add_next_position(player, &next) != 0) {
      printf("player_add_next_position inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  int button_to_click = -1;
  // TODO: add here conditions such as
  // if game_state == PLAYING, process game_playing_buttons->buttons
  // else if game_state == FINISHED, process game_finished_buttons->buttons
  // else nothign (no processing of clicks)
  buttons_array_t *buttons = get_game_buttons(app_state);
  if (buttons != NULL) {
    if (process_buttons_clicks(buttons, before, next, &button_to_click)) {
      printf("process_buttons_clicks inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (button_to_click != -1) {
      button_t pressed_button = game_playing_buttons->buttons[button_to_click];
      ser_add_button_click_to_transmitter_queue(button_to_click);
      pressed_button.onClick(&pressed_button);
    }
  }
  // else {
  //   printf("BUTTONS ARE NULL - might be in waiting\n");
  // }
  return EXIT_SUCCESS;
}

int (game_draw_canvas)(canvas_t *canvas, player_drawer_t *player_drawer){
  drawing_position_t drawing_position;
  drawing_position_t last_position;
  
  player_t *player = player_drawer_get_player(player_drawer);
  if (player == NULL) {
    printf("player is null\n");
    return EXIT_FAILURE;
  }
  brush_t *brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) {
    printf("brush is null\n");
    return EXIT_FAILURE;
  }

  while (player_get_next_position(player, &drawing_position) == OK) {
    set_needs_update(true);
    if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;
    
    draw_in_canvas(canvas, brush, last_position.position, drawing_position);
    // if (game_state == PLAYING) {
    //   draw_in_canvas(canvas, brush, last_position.position, drawing_position);
    // }

    player_set_last_position(player, drawing_position);
  }
  return EXIT_SUCCESS;
}

int (game_process_serial)() {
  player_type_t role = player_drawer_get_role(player_drawer);
  // the serial port function would have to be dependent on the game state
  // example problematic sequence: POSITION CHANGE_GAME_STATE BUTTON_CLICK all on the same call to the function
  // it will have the wrong buttons to click (the ones from the previous state)
  // we passed game_playing_buttons->buttons because the state was PLAYING but then the state change to FINISHED so it should click those other buttons and not the ones from the previous state
  if (role == OTHER_PLAYER) {
    // ser_read_bytes_from_receiver_queue(player_drawer, game_playing_buttons->buttons, NUMBER_game_playing_buttons->buttons);
  }
  return EXIT_SUCCESS;
}

int (game_draw_buttons)() {
  if (game_state == FINISHED) {
    if (vg_draw_buttons(game_finished_buttons)) {
      printf("vg_draw_buttons inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  if (vg_draw_buttons(game_playing_buttons)) {
    printf("vg_draw_buttons inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (game_draw)() {
  if (game_draw_canvas(canvas, player_drawer) != OK) {
    printf("draw_to_canvas inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (/*buffers_need_update()*/ true) {
    if (vg_copy_canvas_buffer(get_buffer(canvas)) != OK) {
      printf("vg_copy_canvas_buffer inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (game_draw_buttons() != OK) {
      printf("game_draw_buttons inside %s\n", __func__);
      return EXIT_FAILURE;
    }

    if (vg_draw_rectangle(GUESS_BOX_X,GUESS_BOX_Y, GUESS_BOX_WIDTH, GUESS_BOX_HEIGHT, BLACK)) return EXIT_FAILURE;
    switch (player_drawer_get_role(player_drawer)){
      case SELF_PLAYER:
        if (vg_draw_text(prompt, GUESS_POS_X, GUESS_POS_Y) != OK) return EXIT_FAILURE;
        break;
      case OTHER_PLAYER:
        if (vg_draw_guess(guess, GUESS_POS_X, GUESS_POS_Y) != OK) return EXIT_FAILURE;
        break;
    }
    if (vg_draw_text(byte_to_str(round_timer),ROUND_TIMER_X, ROUND_TIMER_Y) != OK){
      printf("vg_draw_text inside %s\n", __func__);
      return EXIT_FAILURE;
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

// TODO: FIX THIS
buttons_array_t *(get_game_buttons)(state_t* state) {
  if (game_state == PLAYING) {
    return game_playing_buttons;
  }
  if (game_state == FINISHED) {
    return game_finished_buttons;
  }
  return NULL;
}
