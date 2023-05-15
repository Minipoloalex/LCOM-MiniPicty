#include <stdio.h>
#include <lcom/lcf.h>

#include "../../devices/video_card/video_card.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../model/button/button.h"
#include "player_menu/player_menu.h"

void (setup_menu)();
int (draw_menu_buttons)();
int (draw_menu)();
int (menu_process_mouse)();

void (destroy_menu)();
