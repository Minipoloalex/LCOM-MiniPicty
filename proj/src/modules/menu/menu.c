#include "menu.h"

uint16_t menu_mode = 0x105;
vbe_mode_info_t menu_screen_info;

int (setup_menu)(){
  if(vbe_get_mode_info(menu_mode, &menu_screen_info)) return 1;
  if(map_phys_mem_to_virtual(menu_mode)) return 1;
  if(vg_enter(menu_mode)) return 1;
  return 0;
}

void (setup_buttons)(){
  uint16_t x = menu_screen_info.XResolution / 3;
  uint16_t width = menu_screen_info.XResolution / 3;
  uint16_t height = menu_screen_info.YResolution / 7;

  uint8_t color = 10;

  struct Button play_button = {x, height, width, height, color, "Play"};
  struct Button settings_button = {x, height * 3, width, height, color, "Leaderboard"};
  struct Button exit_button = {x, height * 5, width, height, color, "Exit"};

  buttons[0] = play_button;
  buttons[1] = settings_button;
  buttons[2] = exit_button;
}

int draw_buttons(){
  for(int i = 0; i < 3; i++){
    if(vg_draw_rectangle(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, buttons[i].color)) return 1;

    // draw text
  }
  return 0;
}

void (draw_menu)(){
  if(setup_menu()){
    printf("Error setting up menu\n");
    return;
  }
  setup_buttons();
  if(draw_buttons()){
    printf("Error drawing buttons\n");
    return;
  }
}
