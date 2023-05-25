#include <stdio.h>
#include <lcom/lcf.h>

#include "../../model/state/state.h"
#include "../../devices/video_card/video_card.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../model/button/button.h"
#include "player_menu/player_menu.h"
#include "../../model/cursor/cursor.h"

int (setup_menu)();

int (draw_player_menu)();

int (menu_draw)();
int (menu_process_mouse)();
int (menu_process_serial)();
void (transitionToMenu)(state_t* state);

void (destroy_menu)();
int (calculate_sun_height)(int hour);

//TODO: move thiss
int (is_cursor_over_menu_button)(position_t mouse_position);
