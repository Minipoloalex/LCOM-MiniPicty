#include "menu.h"

#define NUMBER_MENU_BUTTONS 3 /**< @brief Number of menu buttons */
#define HOVERED_BG_COLOR 0x555555 /**< @brief Hovered button background color */
#define NOT_HOVERED_BG_COLOR 0x000000 /**< @brief Not hovered button background color */
#define BROWN_COLOR 0xCD853F /**< @brief Brown color */
#define TIMER_WIDTH 230 /**< @brief Width of the timer */
#define TIMER_HEIGHT 55 /**< @brief Height of the timer */
#define TIMER_MARGIN 10 /**< @brief Margin of the timer */

static buttons_array_t *buttons_array;

static player_menu_t *player_menu;

static state_t *app_state;
static Resources* app_resources;

Background* background_scene;
static char* time_str;

/*==================================================================*/
/**
 * @brief Enter the game button callback
 * 
 * @param button 
 */
void (enter_game)(button_t *button){
  transition_to_game(app_state, false);
}
/**
 * @brief Enter the hard mode game button callback
 * 
 * @param button 
 */
void (enter_hard_mode)(button_t *button){
  transition_to_game(app_state, true);
}
/**
 * @brief Quit the application button callback
 * 
 * @param button 
 */
void (quit_app)(button_t *button) {
  app_state->running_app = false;
}
/**
 * @brief Draw the menu buttons
 * 
 */
int (menu_draw_buttons)();

/**
 * @brief Draw current time in the menu
 * 
 */
int (draw_time)();

/**
 * @brief Draw the menu background
 * Draws the background buffer to the memory buffer
 */
int (draw_background_scene)();
/*==================================================================*/

void (transition_to_menu)(state_t* state){
  default_implementation(state);
  state->draw = menu_draw;
  state->process_mouse = menu_process_mouse;
  state->process_serial = menu_process_serial;
  state->process_rtc = menu_process_rtc;
  uint8_t tmp_hour;
  if (time_str == NULL) {
    rtc_read_temp_hour(&tmp_hour);
    draw_timelapse(background_scene, tmp_hour);
  }
  else {
    draw_timelapse(background_scene, rtc_get_hour());
  }
  state->get_buttons = menu_get_buttons;
}

int (setup_menu)(state_t *state, Resources* resources) {
  app_state = state;
  app_resources = resources;
  player_menu = create_player_menu();
  if (player_menu == NULL) {
    printf("create_player_menu inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  buttons_array = create_buttons_array(NUMBER_MENU_BUTTONS);
  if (buttons_array == NULL) {
    destroy_player_menu(player_menu);
    printf("create_buttons_array inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  uint16_t x = get_h_res() / 3;
  uint16_t width = get_h_res() / 3;
  uint16_t height = get_v_res() / 7;

  button_t *play_button = create_button(x, height, width, height, NOT_HOVERED_BG_COLOR, "PLAY", NO_ICON, enter_game);
  button_t *settings_button = create_button(x, height * 3, width, height, NOT_HOVERED_BG_COLOR, "PLAY HARD MODE", NO_ICON, enter_hard_mode);
  button_t *exit_button = create_button(x, height * 5, width, height, NOT_HOVERED_BG_COLOR, "EXIT", NO_ICON, quit_app);

  buttons_array->buttons[0] = play_button;
  buttons_array->buttons[1] = settings_button;
  buttons_array->buttons[2] = exit_button;

  background_scene = create_background();
  if(background_scene == NULL){
    printf("background_scene inside %s\n", __func__);
    destroy_player_menu(player_menu);
    destroy_buttons_array(buttons_array);
  }
  return EXIT_SUCCESS;
}

int (update_player_menu)() {
  player_t *player = player_menu_get_player(player_menu);
  drawing_position_t drawing_position;
  drawing_position_t last_position;

  while (player_get_next_position(player, &drawing_position) == OK) {
    if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;
    set_needs_update(true);
    player_set_last_position(player, drawing_position);
  }
  return EXIT_SUCCESS;
}

int (menu_draw_buttons)() {
  drawing_position_t last_position;
  player_t *player = player_menu_get_player(player_menu);
  if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;

  int num_buttons = buttons_array->num_buttons;
  for(int i = 0; i < num_buttons; i++){
    button_t *button = buttons_array->buttons[i];
    if(is_cursor_over_button(button, last_position.position)){
      change_button_color(button, HOVERED_BG_COLOR);
    } else {
      change_button_color(button, NOT_HOVERED_BG_COLOR);
    }
  }
  if (vg_draw_buttons(buttons_array, app_resources->font, app_resources->icons) != OK) {
    printf("vg_draw_buttons inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (draw_time)(){
  int x = get_h_res() - TIMER_WIDTH - TIMER_MARGIN;
  if(vg_draw_rectangle(x, TIMER_MARGIN, TIMER_WIDTH, TIMER_HEIGHT, BROWN_COLOR)){
    printf("vg_draw_rectangle inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (time_str != NULL) {
    if(vg_draw_text(time_str, x+TIMER_MARGIN, 2*TIMER_MARGIN, app_resources->font)){
      printf("vg_draw_text inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int (draw_background_scene)(){
  return vg_draw_buffer(background_scene);
}

int (menu_draw)(){
  if (update_player_menu()) {
    printf("update_player_menu inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (buffers_need_update()) { 
    if(draw_background_scene()){
      printf("draw_background inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if(draw_time()){
      printf("draw_time inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if(menu_draw_buttons()) {
      printf("Error drawing buttons\n");
      return EXIT_FAILURE;
    }
    player_t *player = player_menu_get_player(player_menu);
    drawing_position_t position = player_get_current_position(player);

    if(vg_draw_sprite(app_resources->cursors[0], position.position.x, position.position.y)){
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

int (menu_process_mouse)() {
  player_t *player = player_menu_get_player(player_menu);
  drawing_position_t before = player_get_current_position(player);
  drawing_position_t next = mouse_get_drawing_position_from_packet(before.position);

  int button_to_click = -1;
  if (process_buttons_clicks(buttons_array, before, next, &button_to_click)) {
    printf("process_buttons_clicks inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (button_to_click != -1) {
    button_t *pressed_button = buttons_array->buttons[button_to_click];
    if (pressed_button->text == NULL || strcmp(pressed_button->text, "EXIT") != 0) {
      // if the button is not the exit button, we need to add the button click to the transmitter queue
      ser_add_button_click_to_transmitter_queue(button_to_click);  
    }
    pressed_button->onClick(pressed_button);
    set_needs_update(true);
  }

  return player_add_next_position(player, &next);
}

void (destroy_menu)() {
  destroy_player_menu(player_menu);
  destroy_buttons_array(buttons_array);
  destroy_background(background_scene);
}

int (menu_process_serial)() {
  if (ser_read_bytes_from_receiver_queue(NULL, app_state, NULL) != OK) {
    printf("ser_read_bytes_from_receiver_queue inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  set_needs_update(true);
  return EXIT_SUCCESS;
}

buttons_array_t *(menu_get_buttons)(state_t *state){
  return buttons_array;
}

int (menu_process_rtc)() {
  if (time_str != NULL) {
    uint8_t h1 = time_str[0] - '0';
    uint8_t h2 = time_str[1] - '0';
    uint8_t hour = h1 * 10 + h2;
    if (rtc_get_hour() != hour) {
      draw_timelapse(background_scene, rtc_get_hour());
    }
  }
  else {
    draw_timelapse(background_scene, rtc_get_hour());
  }
  time_str = rtc_get_current_time();
  set_needs_update(true);
  return EXIT_SUCCESS;
}
