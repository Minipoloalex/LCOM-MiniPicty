#pragma once
#include "../../../model/player/player.h"

typedef struct PlayerMenu player_menu_t;

player_menu_t *(create_player_menu)();
void (destroy_player_menu)(player_menu_t * player_menu);
player_t *(player_menu_get_player)(player_menu_t * player_menu);
