#include <stdio.h>
#include <lcom/lcf.h>

#include "../../devices/video_card/video_card.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../model/button/button.h"
#include "player_menu/player_menu.h"

void (setup_menu)();

void (draw_menu)();
int (draw_player_menu)();

int (menu_process_mouse)();
int (menu_process_serial)();

void (destroy_menu)();

