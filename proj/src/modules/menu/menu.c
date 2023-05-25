#include "menu.h"

#define NUMBER_MENU_BUTTONS 3

#define HOVERED_BG_COLOR 10
#define HOVERED_TEXT_COLOR 5

#define NOT_HOVERED_BG_COLOR 5
#define NOT_HOVERED_TEXT_COLOR 10

static buttons_array_t *buttons_array;

static player_menu_t *player_menu;

extern vbe_mode_info_t vmi;
static state_t *app_state;

void (enter_game)(button_t* button){
  transition_to_game(app_state);
}

void (transition_to_menu)(state_t* state){
  defaultImplementation(state);
  state->draw = menu_draw;
  state->process_mouse = menu_process_mouse;
  state->process_serial = menu_process_serial;
}

int (setup_menu)(state_t *state) {
  app_state = state;
  player_menu = create_player_menu();
  if (player_menu == NULL) {
    printf("create_player_menu inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  buttons_array = create_buttons_array(NUMBER_MENU_BUTTONS);
  if (buttons_array == NULL) {
    printf("create_buttons_array inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  uint16_t x = vmi.XResolution / 3;
  uint16_t width = vmi.XResolution / 3;
  uint16_t height = vmi.YResolution / 7;

  uint8_t background_color = NOT_HOVERED_BG_COLOR;  
  uint8_t text_color = NOT_HOVERED_TEXT_COLOR;

  button_t play_button = {x, height, width, height, background_color, text_color, "PLAY", enter_game};  
  button_t settings_button = {x, height * 3, width, height, background_color, text_color, "LEADERBOARD", enter_game};
  button_t exit_button = {x, height * 5, width, height, background_color, text_color, "EXIT", enter_game};
  buttons_array->buttons[0] = play_button;
  buttons_array->buttons[1] = settings_button;
  buttons_array->buttons[2] = exit_button;
  return EXIT_SUCCESS;
}

int (draw_player_menu)() {
  player_t *player = player_menu_get_player(player_menu);
  drawing_position_t drawing_position;
  drawing_position_t last_position;

  while (player_get_next_position(player, &drawing_position) == OK) {
    if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;
    set_needs_update(true);
    if (drawing_position.is_drawing) {
      // TODO: check for collisions with buttons
    }
    player_set_last_position(player, drawing_position);
  }
  position_t position = player_get_current_position(player).position;

  for (int i = 0; i < NUMBER_MENU_BUTTONS; i++) {
    button_t button = buttons_array->buttons[i];
    if(is_cursor_over_button(button, position)){
      change_button_colors(&button, HOVERED_BG_COLOR, HOVERED_TEXT_COLOR);
    } else {
      change_button_colors(&button, NOT_HOVERED_BG_COLOR, NOT_HOVERED_TEXT_COLOR);
    }
  }
  return EXIT_SUCCESS;
}
int (draw_buttons)() {
  drawing_position_t last_position;
  player_t *player = player_menu_get_player(player_menu);
  if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;

  int num_buttons = buttons_array->num_buttons;
  printf("Number of buttons: %d \n", num_buttons);
  for(int i = 0; i < num_buttons; i++){
    button_t *button = &buttons_array->buttons[i];
    if(is_cursor_over_button(*button, last_position.position)){
      printf("on if\n");
      change_button_colors(button, 10, 5);
    } else {
      printf("on else\n");
      change_button_colors(button, 5, 10);
    }
  }
  if (vg_draw_buttons(buttons_array) != OK) {
    printf("vg_draw_buttons inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (draw_sun)(){
  int hour = 9;
  int hour_space = (vmi.XResolution - 180) / 13;
  hour -= 6;
  int x = (vmi.XResolution / 2) - 6*hour_space + (hour)*hour_space;
  int y = calculate_sun_height(x);
  if(vg_draw_circle(x-30, vmi.YResolution - 300 - (y * 15), 60, 62)){
    printf("vg_draw_circle inside %s\n", __func__);
    return;
  }
}

void (draw_stars)(){
  srand(time(0));
  int x = 0, y = 0;
  for(int i=0; i<100; i++){
    x = rand() % vmi.XResolution;
    y = rand() % vmi.YResolution;
    if(vg_draw_circle(x, y, 2, 63)){
      printf("vg_draw_pixel inside %s\n", __func__);
      return;
    }
  }
}

void (draw_sky)(){
  //TODO: change color based on hour
  int blue = 11;
  //int black = 0;
  int terrain_height = 300;
  if(vg_draw_rectangle(0, 0, vmi.XResolution, vmi.YResolution - terrain_height, blue)){
    printf("vg_draw_rectangle inside %s\n", __func__);
    return;
  }

  //TODO: draw stars if night
  //draw_stars();
}

void (draw_terrain)(){
  int terrain_height = 300;
  if(vg_draw_rectangle(0, vmi.YResolution - terrain_height, vmi.XResolution, vmi.YResolution, 20)){
    printf("vg_draw_rectangle inside %s\n", __func__);
    return;
  }
}

void (draw_timelapse)(){
  draw_sky();
  draw_sun();
  draw_terrain();
}

int (menu_draw)(){
  draw_timelapse();

  if (draw_player_menu()) {
    printf("draw_player_menu inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (buffers_need_update()) {  // if no new mouse positions, don't update anything
    if(draw_buttons()) {
      printf("Error drawing buttons\n");
      return EXIT_FAILURE;
    }
    player_t *player = player_menu_get_player(player_menu);
    drawing_position_t position = player_get_current_position(player);

    if (vg_draw_cursor(0, position.position) != OK) {
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
    button_t pressed_button = buttons_array->buttons[button_to_click];
    ser_add_button_click_to_transmitter_queue(button_to_click);
    pressed_button.onClick(&pressed_button);
  }

  return player_add_next_position(player, &next);
}

void (destroy_menu)() {
  destroy_player_menu(player_menu);
  vg_clear_buffers();
}

int (menu_process_serial)() {
  return EXIT_SUCCESS;
}

int (is_cursor_over_menu_button)(position_t mouse_position){
  for(int i = 0; i < NUMBER_MENU_BUTTONS; i++){
    if(is_cursor_over_button(buttons_array->buttons[i], mouse_position)){
      return i;
    }
  }
  return -1;
}


//FIX this function
int (calculate_sun_height)(int x){
  x = x - (vmi.XResolution / 2);
  int a = 10;
  int b = 2;

  // Using ellipse formula to calculate y based on x
  return (int)(sqrt(1 - pow((x), 2) / pow(a, 2)) * pow(b, 2));
}

buttons_array_t *(get_menu_buttons)(){
  return buttons_array;
}
