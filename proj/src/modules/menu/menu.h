#include <stdio.h>
#include <lcom/lcf.h>

#include "player_menu/player_menu.h"

#include "../../devices/video_card/video_card.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../devices/rtc/rtc.h"

#include "../../model/button/button.h"
#include "../../model/cursor/cursor.h"
#include "../../model/state/state.h"

int (setup_menu)();

void (draw_menu)();
int (draw_player_menu)();

int (menu_process_mouse)();
int (menu_process_serial)();
int (menu_process_rtc)();

void (destroy_menu)();

//TODO: move thiss
int (is_cursor_over_menu_button)(position_t mouse_position);
