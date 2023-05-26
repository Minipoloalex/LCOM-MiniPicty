#include "menu.h"

#define NUMBER_MENU_BUTTONS 3

#define HOVERED_BG_COLOR 0x555555
#define NOT_HOVERED_BG_COLOR 0x000000

static buttons_array_t *buttons_array;

static player_menu_t *player_menu;

static state_t *app_state;

uint8_t* background_scene;

uint32_t (calculate_sky_color)(int hour);
void (draw_timelapse)(int hour);


void (enter_game)(button_t *button){
  transition_to_game(app_state);
}

void (quit_app)(button_t *button) {
  app_state->running_app = false;
}

void (transition_to_menu)(state_t* state){
  default_implementation(state);
  state->draw = menu_draw;
  state->process_mouse = menu_process_mouse;
  state->process_serial = menu_process_serial;
  draw_timelapse(22); //TODO: dynamic change hour here
  state->get_buttons = menu_get_buttons;
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
    destroy_player_menu(player_menu);
    printf("create_buttons_array inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  uint16_t x = get_h_res() / 3;
  uint16_t width = get_h_res() / 3;
  uint16_t height = get_v_res() / 7;

  button_t *play_button = create_button(x, height, width, height, NOT_HOVERED_BG_COLOR, "PLAY", NO_ICON, enter_game);
  button_t *settings_button = create_button(x, height * 3, width, height, NOT_HOVERED_BG_COLOR, "PLAY HARD MODE", NO_ICON, enter_game);
  button_t *exit_button = create_button(x, height * 5, width, height, NOT_HOVERED_BG_COLOR, "EXIT", NO_ICON, quit_app);

  buttons_array->buttons[0] = play_button;
  buttons_array->buttons[1] = settings_button;
  buttons_array->buttons[2] = exit_button;

  background_scene = (uint8_t *) malloc(get_vram_size() * sizeof(uint8_t));
  if(background_scene == NULL){
    printf("background_scene inside %s\n", __func__);
    destroy_player_menu(player_menu);
    destroy_buttons_array(buttons_array);
  }

  return EXIT_SUCCESS;
}

int (draw_player_menu)() {
  player_t *player = player_menu_get_player(player_menu);
  drawing_position_t drawing_position;
  drawing_position_t last_position;

  while (player_get_next_position(player, &drawing_position) == OK) {
    if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;
    set_needs_update(true);
    player_set_last_position(player, drawing_position);
  }
  position_t position = player_get_current_position(player).position;

  for (int i = 0; i < NUMBER_MENU_BUTTONS; i++) {
    button_t *button = buttons_array->buttons[i];
    if(is_cursor_over_button(button, position)){
      change_button_color(button, HOVERED_BG_COLOR);
    } else {
      change_button_color(button, NOT_HOVERED_BG_COLOR);
    }
  }
  return EXIT_SUCCESS;
}
int (draw_buttons)() {
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
  if (vg_draw_buttons(buttons_array) != OK) {
    printf("vg_draw_buttons inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (draw_sun)(uint32_t hour){
  int hour_space = (get_h_res() - 180) / 13;
  hour -= 6;
  int x = (get_h_res() / 2) - 6*hour_space + (hour)*hour_space;
  int y = calculate_sun_height(x);
  uint32_t color = 0xFFFF00;
  if(vg_draw_circle_to_buffer(background_scene, x-30, get_v_res() - 300 - (y * 15), 60, color)){
    printf("vg_draw_circle inside %s\n", __func__);
    return;
  }
}

void (draw_stars)(){
  srand(time(0));
  int x = 0, y = 0;
  for(int i=0; i<100; i++){
    x = rand() % get_h_res();
    y = rand() % get_v_res();
    if(vg_draw_circle_to_buffer(background_scene, x, y, 2, 0xFFFFFF)){
      printf("vg_draw_pixel inside %s\n", __func__);
      return;
    }
  }
}

void (draw_sky)(uint32_t hour){
  uint32_t color = calculate_sky_color(hour);
  int terrain_height = 300;
  if(vg_draw_rectangle_to_buffer(background_scene, 0, 0, get_h_res(), get_v_res() - terrain_height, color)){
    printf("vg_draw_rectangle inside %s\n", __func__);
    return;
  }

  if(hour > 19){
    draw_stars();
  }
}

void (draw_terrain)(){
  int terrain_height = 300;
  uint32_t brown = 0xCD853F;
  if(vg_draw_rectangle_to_buffer(background_scene, 0, get_v_res() - terrain_height, get_h_res(), get_v_res(), brown)){
    printf("vg_draw_rectangle inside %s\n", __func__);
    return;
  }
}

int (draw_time)(){
  int space = 10;
  int width = 155;
  int x = get_h_res() - width - space;
  uint32_t brown = 0xCD853F;
  
  //int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
  if(vg_draw_rectangle(x, space, width, 55, brown)){
    printf("vg_draw_rectangle inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  //int (vg_draw_text)(char *string, uint16_t x, uint16_t y){
  if(vg_draw_text("12 00", x+space, 2*space)){
    printf("vg_draw_text inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (draw_timelapse)(int hour){
  draw_sky(hour);
  draw_sun(hour);
  draw_terrain();
}

int (draw_background_scene)(){
  return vg_draw_buffer(background_scene);
}

int (menu_draw)(){

  if (draw_player_menu()) {
    printf("draw_player_menu inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (/*buffers_need_update()*/ true) {  // if no new mouse positions, don't update anything
    if(draw_background_scene()){
      printf("draw_background inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if(draw_time()){
      printf("draw_time inside %s\n", __func__);
      return EXIT_FAILURE;
    }
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
    button_t *pressed_button = buttons_array->buttons[button_to_click];
    ser_add_button_click_to_transmitter_queue(button_to_click);
    pressed_button->onClick(pressed_button);
  }

  return player_add_next_position(player, &next);
}

void (destroy_menu)() {
  destroy_player_menu(player_menu);
  free(background_scene);
  destroy_buttons_array(buttons_array);
}

int (menu_process_serial)() {
  if (ser_read_bytes_from_receiver_queue(NULL, app_state) != OK) {
    printf("ser_read_bytes_from_receiver_queue inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

//FIX this function
int (calculate_sun_height)(int x){
  x = x - (get_h_res() / 2);
  int a = 10;
  int b = 2;

  // Using ellipse formula to calculate y based on x
  return (int)(sqrt(1 - pow((x), 2) / pow(a, 2)) * pow(b, 2));
}

uint32_t (calculate_sky_color)(int hour){
  if(hour >= 19 || hour <= 6) return 0x242424;

  if(hour > 12){
    hour = 12 - (hour - 12);
  }

  // default blue: 0x00F0FF
  uint32_t red = 0x00;
  uint32_t blue = 0xF0;
  uint32_t green = 0xFF;

  green = (hour * 255) / 12;
  blue = (hour * 255) / 12;

  return (red << 16) | (green << 8) | blue;
}


buttons_array_t *(menu_get_buttons)(state_t *state){
  return buttons_array;
}
