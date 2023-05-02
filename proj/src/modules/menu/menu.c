#include "menu.h"

uint16_t menu_mode = GRAPHICS_MODE_0;
extern vbe_mode_info_t vmi;

void (setup_menu_buttons)(){
  uint16_t x = vmi.XResolution / 3;
  uint16_t width = vmi.XResolution / 3;
  uint16_t height = vmi.YResolution / 7;

  uint8_t background_color = 10;  
  uint8_t text_color = 5;

  struct button play_button = {x, height, width, height, background_color, text_color, "PLAY"};
  struct button settings_button = {x, height * 3, width, height, background_color, text_color, "LEADERBOARD"};
  struct button exit_button = {x, height * 5, width, height, background_color, text_color, "EXIT"};

  menu_buttons[0] = play_button;
  menu_buttons[1] = settings_button;
  menu_buttons[2] = exit_button;
}

int (draw_buttons)(){
  for(int i = 0; i < 3; i++){
    draw_button(&menu_buttons[i]);
  }
  return 0;
}

void (draw_menu)(){
  if(draw_buttons()){
    printf("Error drawing buttons\n");
    return;
  }
}
