#include <stdio.h>
#include <lcom/lcf.h>

#include "../../devices/video_card/video_card.h"
#include "../../model/button.h"

int (setup_menu)();
void (setup_buttons)();
int (draw_buttons)();
void (draw_menu)();

struct button menu_buttons[3];
