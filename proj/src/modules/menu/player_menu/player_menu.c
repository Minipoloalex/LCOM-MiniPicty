#include "player_menu.h"

/**
 * @brief Player Menu struct
 * Contains a player
 * 
 */
struct PlayerMenu {
  player_t *player;
};

player_menu_t *(create_player_menu)() {
  player_menu_t *player_menu = malloc(sizeof(player_menu_t));
  if (player_menu == NULL) {
    return NULL;
  }
  player_menu->player = create_player();
  if (player_menu->player == NULL) {
    free(player_menu);
    return NULL;
  }
  return player_menu;
}

void (destroy_player_menu)(player_menu_t * player_menu) {
  destroy_player(player_menu->player);
}

player_t *(player_menu_get_player)(player_menu_t * player_menu) {
  return player_menu->player;
}
