#include "game.h"

#define GRAY_COLOR 0xA0A0A0 /**< @brief Gray color */
#define NUMBER_GAME_PLAYING_BUTTONS 13 /**< @brief Number of game playing buttons */
#define NUMBER_GAME_FINISHED_BUTTONS 3 /**< @brief Number of game finished buttons */
#define GAME_WAITING_TIME 5 /**< @brief Time to wait for the drawer to draw */
#define GAME_ROUND_TIME 60 /**< @brief Time to draw and guess */
#define WON_TEXT "You won" /**< @brief Text to display on win */
#define LOSE_TEXT "You lost" /**< @brief Text to display on lose */
#define YOU_ARE_DRAWING_TEXT "Get ready you are drawing" /**< @brief Text to display when the player is drawing */
#define YOU_ARE_GUESSING_TEXT "Get ready you are guessing" /**< @brief Text to display when the player is guessing */
#define FINISH_TEXT_SIZE (MAX(sizeof(WON_TEXT), sizeof(LOSE_TEXT)) + 1) /**< @brief Size of the finish text for allocation purposes */

/**
 * @brief Sub state types of game state
 */
typedef enum game_state {
  PLAYING,
  WAITING,
  FINISHED
} game_state_t;

static Resources* app_resources;
static buttons_array_t *game_playing_buttons;
static buttons_array_t *game_finished_buttons;

static player_drawer_t *player_drawer;
static canvas_t *canvas;
static guess_word_t *guess;
static char prompt[15];
static uint8_t round_timer;
static game_state_t game_state;
static char *finish_text;
static state_t *app_state;
static bool is_hard_mode;
static asteroid_t *asteroid;

static int cell_width = 0;
static int cell_height = 0;

/*==================================================================*/

/**
 * @brief Draw the game buttons
 * @return int 0 if success, 1 otherwise
 */

int(game_draw_buttons)();

/**
 * @brief Draw to the canvas in the game state
 * @return int 0 if success, 1 otherwise
*/
int(game_draw_canvas)(canvas_t *canvas, player_drawer_t *player_drawer);

/**
 * @brief Play again button callback
 * Set the game state to WAITING and reset the game by clearing the canvas and generating a new prompt
 * 
 * @param button 
 */
void(play_again)(button_t *button) {
  if (game_state != FINISHED) {
    printf("Wrong game state inside %s\n", __func__);
    return;
  }
  game_state = WAITING;
  round_timer = GAME_WAITING_TIME;
  canvas_clear(canvas);
  if (player_drawer_get_role(player_drawer) == SELF_PLAYER) {
    uint8_t word_index = prompt_generate(prompt);
    ser_add_word_index(word_index);
    app_state->word_index = word_index;
  }
  reset_guess_word(guess);
  if (is_hard_mode) {
    asteroid_reset_position(asteroid);
  }
  set_needs_update(true);
}
/**
 * @brief Change roles and play again button callback
 * Change the roles of the players and call play_again
 * 
 * @param button 
 */
void(play_again_change_roles)(button_t *button) {
  player_drawer_change_role(player_drawer);
  play_again(button);
}
/**
 * @brief Quit game button callback
 * Transition to the menu state
 * 
 * @param button 
 */
void(quit_game)(button_t *button) {
  if (game_state != FINISHED) {
    printf("Wrong game state inside %s\n", __func__);
    return;
  }
  transition_to_menu(app_state);
  set_needs_update(true);
}
/**
 * @brief Change brush color button callback
 * 
 * @param button 
 */
void (change_brush_color)(button_t *button) {
  brush_t *brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL)
    return;
  set_brush_color(brush, button->background_color);
}
/**
 * @brief Increase brush size button callback
 * 
 * @param button 
 */
void(increase_brush_size)(button_t *button) {
  brush_t *brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL)
    return;
  brush_increase_size(brush);
}
/**
 * @brief Decrease brush size button callback
 * 
 * @param button 
 */
void(decrease_brush_size)(button_t *button) {
  brush_t *brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL)
    return;
  brush_decrease_size(brush);
}
/**
 * @brief Set brush to rubber button callback
 * 
 * @param button 
 */
void(set_rubber)(button_t *button) {
  brush_t *brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL)
    return;
  set_brush_color(brush, canvas->background_color);
}
/**
 * @brief Clear canvas button callback
 * 
 * @param button 
 */
void(clear_canvas)(button_t *button) {
  if (canvas == NULL) {
    printf("canvas is null inside %s\n", __func__);
    return;
  }
  canvas_clear(canvas);
}
/*==================================================================*/

int(setup_game)(bool isTransmitter, state_t *state, Resources* resources) {
  app_state = state;
  app_resources = resources;
  finish_text = malloc(FINISH_TEXT_SIZE * sizeof(char));
  if (finish_text == NULL) {
    printf("malloc inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  player_drawer = create_player_drawer(isTransmitter ? SELF_PLAYER : OTHER_PLAYER);
  if (player_drawer == NULL) {
    printf("create_player_drawer inside %s\n", __func__);
    free(finish_text);
    return EXIT_FAILURE;
  }
  game_playing_buttons = create_buttons_array(NUMBER_GAME_PLAYING_BUTTONS);
  if (game_playing_buttons->buttons == NULL) {
    destroy_player_drawer(player_drawer);
    free(finish_text);
    printf("create_buttons_array inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  game_finished_buttons = create_buttons_array(NUMBER_GAME_FINISHED_BUTTONS);
  if (game_finished_buttons == NULL) {
    free(finish_text);
    destroy_player_drawer(player_drawer);
    destroy_buttons_array(game_playing_buttons);
    printf("create_buttons_array inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  int min_len = get_h_res() / 9;
  int min_height = get_v_res() / 11;
  cell_width = min_len;
  cell_height = min_height;
  button_t *red_button = create_button(0, 0, min_len, min_height, 0XFF0000, NULL, NO_ICON, change_brush_color);
  button_t *orange_button = create_button(6*min_len, 0, min_len, min_height, 0XFF9933, NULL, NO_ICON, change_brush_color);
  button_t *yellow_button = create_button(3*min_len, 0, min_len, min_height, 0XFFFF00, NULL, NO_ICON,change_brush_color);
  button_t *green_button = create_button(min_len, 0, min_len, min_height, 0X00FF00, NULL, NO_ICON,change_brush_color);
  button_t *light_blue_button = create_button(7*min_len, 0, min_len, min_height, 0XADD8E6, NULL, NO_ICON, change_brush_color);
  button_t *dark_blue_button = create_button(2*min_len, 0, min_len, min_height, 0X0066CC, NULL, NO_ICON,change_brush_color);
  button_t *pink_button = create_button(8*min_len, 0, min_len, min_height, 0XFF99FF, NULL, NO_ICON, change_brush_color);
  button_t *gray_button = create_button(5*min_len, 0, min_len, min_height, GRAY_COLOR, NULL, NO_ICON,change_brush_color);
  button_t *black_button = create_button(4*min_len, 0, min_len, min_height, 0X000000, NULL, NO_ICON,change_brush_color);
  
  game_playing_buttons->buttons[0] = red_button;
  game_playing_buttons->buttons[1] = orange_button;
  game_playing_buttons->buttons[2] = yellow_button;
  game_playing_buttons->buttons[3] = green_button;
  game_playing_buttons->buttons[4] = light_blue_button;
  game_playing_buttons->buttons[5] = dark_blue_button;
  game_playing_buttons->buttons[6] = pink_button;
  game_playing_buttons->buttons[7] = gray_button;
  game_playing_buttons->buttons[8] = black_button;

  int other_buttons_color = GRAY_COLOR;

  button_t * increase_size_button = create_button(8*min_len, 2*min_height, min_len, min_height, other_buttons_color, NULL, PLUS_ICON, increase_brush_size);
  button_t * decrease_size_button = create_button(8*min_len, 4*min_height, min_len, min_height, other_buttons_color, NULL, MINUS_ICON, decrease_brush_size);
  button_t * rubber_button = create_button(8*min_len, 6*min_height, min_len, min_height, other_buttons_color, NULL, RUBBER_ICON, set_rubber);
  button_t * clear_button = create_button(8*min_len, 8*min_height, min_len, min_height, other_buttons_color, NULL, TRASH_ICON, clear_canvas);

  game_playing_buttons->buttons[9] = increase_size_button;
  game_playing_buttons->buttons[10] = decrease_size_button;
  game_playing_buttons->buttons[11] = rubber_button;
  game_playing_buttons->buttons[12] = clear_button;

  uint16_t x_finished = get_h_res() / 3;
  uint16_t width_finished = get_h_res() / 3;
  uint16_t height_finished = get_v_res() / 7;

  button_t *play_again_button = create_button(x_finished, height_finished, width_finished, height_finished, other_buttons_color, "Play Again", NO_ICON, play_again);
  button_t *play_again_change_state = create_button(x_finished, height_finished * 3, width_finished, height_finished, other_buttons_color, "Change Roles", NO_ICON, play_again_change_roles);
  button_t *quit_button = create_button(x_finished, height_finished * 5, width_finished, height_finished, other_buttons_color, "Back Menu", NO_ICON, quit_game);

  game_finished_buttons->buttons[0] = play_again_button;
  game_finished_buttons->buttons[1] = play_again_change_state;
  game_finished_buttons->buttons[2] = quit_button;

  canvas = canvas_init(0, min_height, 8 * min_len, 9 * min_height);
  if (canvas == NULL) {
    destroy_player_drawer(player_drawer);
    free(finish_text);
    destroy_buttons_array(game_playing_buttons);
    destroy_buttons_array(game_finished_buttons);
    return EXIT_FAILURE;
  }

  guess = create_guess_word();
  if (guess == NULL) {
    free(finish_text);
    destroy_player_drawer(player_drawer);
    canvas_destroy(canvas);
    destroy_buttons_array(game_playing_buttons);
    destroy_buttons_array(game_finished_buttons);
    return EXIT_FAILURE;
  }

  asteroid = create_asteroid(app_resources->asteroids);
  if (asteroid == NULL) {
    free(finish_text);
    destroy_player_drawer(player_drawer);
    canvas_destroy(canvas);
    destroy_buttons_array(game_playing_buttons);
    destroy_buttons_array(game_finished_buttons);
    destroy_guess_word(guess);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void(destroy_game)() {
  free(finish_text);
  destroy_player_drawer(player_drawer);
  canvas_destroy(canvas);
  destroy_buttons_array(game_playing_buttons);
  destroy_buttons_array(game_finished_buttons);
  destroy_guess_word(guess);
  destroy_asteroid(asteroid);
}

extern int timer_counter;
void(transition_to_game)(state_t *state, bool hard_mode) {
  timer_counter = 0;
  default_implementation(state);
  state->draw = game_draw;
  state->process_mouse = game_process_mouse;
  state->process_keyboard = game_process_keyboard;
  state->process_serial = game_process_serial;
  state->process_timer = game_process_timer;
  state->get_buttons = game_get_buttons;
  is_hard_mode = hard_mode;
  if (player_drawer_get_role(player_drawer) == SELF_PLAYER) {
    uint8_t word_index = prompt_generate(prompt);
    ser_add_word_index(word_index);
    app_state->word_index = word_index;
  }
  else if (app_state->word_index != 255) {
    /* this is done because the word_index might be received by menu_process_serial */
    get_word_from_index(app_state->word_index, prompt);
  }
  canvas_clear(canvas);
  game_state = WAITING;
  round_timer = GAME_WAITING_TIME;
  if (strcpy(finish_text, LOSE_TEXT) == NULL) {
    printf("strcpy failed inside %s\n", __func__);
  }
  if (is_hard_mode) {
    asteroid_reset_position(asteroid);
  }
}

int (game_process_timer)() {
  if (game_state == FINISHED) {
    return EXIT_SUCCESS;
  }
  if (is_hard_mode && game_state == PLAYING) {
    if (game_move_asteroid(asteroid) != OK) {
      printf("game_move_asteroid failed inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    set_needs_update(true);
  }
  if (timer_counter % sys_hz() == 0) {    
    round_timer--;
    set_needs_update(true);
    if (round_timer == 0) {
      if (game_state == WAITING) {
        game_state = PLAYING;
        round_timer = GAME_ROUND_TIME;
        return EXIT_SUCCESS;
      }
      if (game_state == PLAYING) {
        game_state = FINISHED;
        round_timer = 0;
        if (strcpy(finish_text, LOSE_TEXT) == NULL) {
          printf("strcpy failed inside %s\n", __func__);
          return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
      }
    }
  }
  return EXIT_SUCCESS;
}

extern int keyboard_return_value;
extern uint8_t scancode;
int(game_process_keyboard)() {
  if (player_drawer_get_role(player_drawer) == SELF_PLAYER) {
    return EXIT_SUCCESS;
  }
  if (game_state != PLAYING)
    return EXIT_SUCCESS;

  if (keyboard_return_value)
    return EXIT_SUCCESS;
  if (scancode == 0)
    return EXIT_SUCCESS;

  bool right_guess;
  bool is_break = false;
  switch (scancode) {
    case MAKE_BACKSPACE:
      delete_character(guess);
      break;

    case MAKE_ENTER:
      validate_guess_word(prompt, guess, &right_guess);
      reset_guess_word(guess);
      if (right_guess) {
        ser_add_won_round();
        set_needs_update(true);
        game_state = FINISHED;
        if (strcpy(finish_text, WON_TEXT) == NULL) {
          printf("strcpy failed inside %s\n", __func__);
          return EXIT_FAILURE;
        }
      }
      break;
    default:
      if (is_breakcode(scancode, &is_break))
        return EXIT_FAILURE;

      if (!is_break) {
        uint8_t caracter;
        if (translate_scancode(scancode, &caracter))
          return EXIT_SUCCESS;
        if (write_character(guess, caracter))
          return EXIT_FAILURE;
      }
  }
  set_needs_update(true);
  return EXIT_SUCCESS;
}

int(game_process_mouse)() {
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
  buttons_array_t *buttons = game_get_buttons(app_state);
  if (buttons != NULL) {
    if (process_buttons_clicks(buttons, before, next, &button_to_click)) {
      printf("process_buttons_clicks inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (button_to_click != -1) {
      button_t *pressed_button = buttons->buttons[button_to_click];
      ser_add_button_click_to_transmitter_queue(button_to_click);
      pressed_button->onClick(pressed_button);
      set_needs_update(true);
    }
  }
  return EXIT_SUCCESS;
}

int(game_draw_canvas)(canvas_t *canvas, player_drawer_t *player_drawer) {
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
    if (player_get_last_position(player, &last_position))
      return EXIT_FAILURE;

    if (game_state == PLAYING) {
      draw_in_canvas(canvas, brush, last_position.position, drawing_position, is_hard_mode ? asteroid : NULL);
    }

    player_set_last_position(player, drawing_position);
  }
  return EXIT_SUCCESS;
}

int (game_process_serial)() {
  bool won_round = false;
  ser_read_bytes_from_receiver_queue(player_drawer, app_state, &won_round);

  set_needs_update(true);
  if (app_state->word_index != 255) {
    get_word_from_index(app_state->word_index, prompt);
  }
  if (won_round) {
    game_state = FINISHED;
    if (strcpy(finish_text, WON_TEXT) == NULL) {
      printf("strcpy failed inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int(game_draw_buttons)() {
  if (vg_draw_buttons(game_playing_buttons, app_resources->font, app_resources->icons)) {
    printf("vg_draw_buttons inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (game_state == FINISHED) {
    if (vg_draw_rectangle(0, cell_height, cell_width*4, cell_height, GRAY_COLOR) != OK)
      return EXIT_FAILURE;
    if (vg_draw_text(finish_text, 0, cell_height+cell_height/4, app_resources->font) != OK)
      return EXIT_FAILURE;
    if (vg_draw_rectangle(2*cell_width, cell_height, 5*cell_width, 9*cell_height, BLACK) != OK)
      return EXIT_FAILURE;
    if (vg_draw_buttons(game_finished_buttons, app_resources->font, app_resources->icons)) {
      printf("vg_draw_buttons inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int(game_draw)() {
  if (game_draw_canvas(canvas, player_drawer) != OK) {
    printf("draw_to_canvas inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (buffers_need_update()) {
    if (vg_draw_buffer(get_buffer(canvas)) != OK) {
      printf("vg_draw_buffer inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (game_draw_buttons() != OK) {
      printf("game_draw_buttons inside %s\n", __func__);
      return EXIT_FAILURE;
    }

    if (vg_draw_rectangle(GUESS_BOX_X, GUESS_BOX_Y, GUESS_BOX_WIDTH, GUESS_BOX_HEIGHT, BLACK))
      return EXIT_FAILURE;
    player_type_t role = player_drawer_get_role(player_drawer);
    switch (role) {
      case SELF_PLAYER:
        if (vg_draw_text(prompt, GUESS_POS_X, GUESS_POS_Y, app_resources->font) != OK)
          return EXIT_FAILURE;
        break;
      case OTHER_PLAYER:
        if (vg_draw_guess(guess, GUESS_POS_X, GUESS_POS_Y, app_resources->font) != OK)
          return EXIT_FAILURE;
        break;
    }
    if (game_state == WAITING) {
      if (role == SELF_PLAYER) {
        if (vg_draw_rectangle(0, cell_height, cell_width*5 + 50, cell_height, GRAY_COLOR) != OK)
          return EXIT_FAILURE;
        if (vg_draw_text(YOU_ARE_DRAWING_TEXT, 10, cell_height+cell_height/4, app_resources->font) != OK)
          return EXIT_FAILURE;
      }
      else {
        if (vg_draw_rectangle(0, cell_height, cell_width*5 + 70, cell_height, GRAY_COLOR) != OK)
          return EXIT_FAILURE;
        if (vg_draw_text(YOU_ARE_GUESSING_TEXT, 10, cell_height+cell_height/4, app_resources->font) != OK)
          return EXIT_FAILURE;
      }
    }
    if (vg_draw_text(byte_to_str(round_timer), ROUND_TIMER_X, ROUND_TIMER_Y, app_resources->font) != OK) {
      printf("vg_draw_text inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (game_state == PLAYING && is_hard_mode && draw_asteroid(asteroid) != OK) {
      printf("draw_asteroid inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    player_t *player = player_drawer_get_player(player_drawer);
    drawing_position_t drawing_position = player_get_current_position(player);
    update_cursor(drawing_position.position);
    cursor_type_t cursor = player_drawer_get_cursor(player_drawer);

    if (vg_draw_sprite(app_resources->cursors[cursor], drawing_position.position.x, drawing_position.position.y)) {
      printf("vg_draw_sprite inside %s\n", __func__);
      return EXIT_FAILURE;
    }

    if (vg_buffer_flip()) {
      printf("vg_buffer_flip inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

void(update_cursor)(position_t position) {
  brush_t *brush = player_drawer_get_brush(player_drawer);
  if(!is_inside_rectangle(position, canvas->start_point, canvas->width, canvas->height)){
    player_drawer_set_cursor(player_drawer, POINTER);
  }
  else if (brush->color == canvas->background_color) {
    player_drawer_set_cursor(player_drawer, RUBBER);
  }
  else {
    player_drawer_set_cursor(player_drawer, PEN);
  }
}

buttons_array_t *(game_get_buttons)(state_t *state) {
  if (game_state == PLAYING) { //TODO: Delete the brackets
    return game_playing_buttons;
  }
  if (game_state == FINISHED) {
    return game_finished_buttons;
  }
  return NULL;
}


int (game_move_asteroid)(asteroid_t *asteroid) {
  if (game_state != PLAYING) {
    printf("game_state != PLAYING inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  bool hit_left_wall = false;
  /* This is needed because uint16_t makes -2 be 0xFF... */
  if (asteroid->x_speed < 0 && asteroid->position.x < abs(asteroid->x_speed)) {
    hit_left_wall = true;
  }
  else asteroid->position.x += asteroid->x_speed;
  asteroid->position.y += asteroid->y_speed;
  
  Sprite *xpm = asteroid->xpms[asteroid->current_xpm];
  uint16_t asteroid_right_side = asteroid->position.x + xpm->width;
  uint16_t canvas_right_side = canvas->start_point.x + canvas->width;
  if (asteroid_right_side >= canvas_right_side || hit_left_wall) {
    asteroid->x_speed *= -1;
    if (asteroid_right_side >= canvas_right_side) {  /* hit the right wall */
      asteroid->position.x = canvas_right_side - xpm->width - 1;
      asteroid->current_xpm = asteroid->y_speed > 0 ? DOWN_LEFT : UP_LEFT;
    }
    else {  /* hit the left wall */
      asteroid->position.x = canvas->start_point.x;
      asteroid->current_xpm = asteroid->y_speed > 0 ? DOWN_RIGHT : UP_RIGHT;
    }
  }

  uint16_t asteroid_bottom_side = asteroid->position.y + xpm->height;
  uint16_t canvas_bottom_side = canvas->start_point.y + canvas->height;
  if (asteroid_bottom_side >= canvas_bottom_side || asteroid->position.y < canvas->start_point.y) {
    asteroid->y_speed *= -1;
    if (asteroid_bottom_side >= canvas_bottom_side) { /* hit the bottom wall */
      asteroid->position.y = canvas_bottom_side - xpm->height - 1;
      asteroid->current_xpm = asteroid->x_speed > 0 ? UP_RIGHT : UP_LEFT;
    }
    else {  /* hit the top wall */
      asteroid->position.y = canvas->start_point.y;
      asteroid->current_xpm = asteroid->x_speed > 0 ? DOWN_RIGHT : DOWN_LEFT;
    }
  }
  return EXIT_SUCCESS;
}

