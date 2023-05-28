#include "player_drawer.h"

/**
 * @brief Player Drawer struct
 * Contains a player, a player type, a brush and a cursor
 * 
 */
struct PlayerDrawer {
  player_t *player;

  player_type_t player_state;
  brush_t *brush;
  cursor_type_t cursor;
};

void (destroy_player_drawer)(player_drawer_t *player_drawer) {
  destroy_brush(player_drawer->brush);
  destroy_player(player_drawer->player);
  free(player_drawer);
}

brush_t *player_drawer_get_brush(player_drawer_t *player_drawer) {
  return player_drawer->brush;
}

player_type_t (player_drawer_get_role)(player_drawer_t *player_drawer) {
  return player_drawer->player_state;
}

int (player_drawer_change_role)(player_drawer_t *player_drawer) {
  player_drawer->player_state = (player_drawer->player_state == SELF_PLAYER) ? OTHER_PLAYER : SELF_PLAYER;
  return EXIT_SUCCESS;
}

player_drawer_t *(create_player_drawer)(player_type_t player_type) {
  player_drawer_t *player_drawer = malloc(sizeof(player_drawer_t));
  if (player_drawer == NULL) return NULL;

  player_drawer->player = create_player();
  if (player_drawer->player == NULL) {
    free(player_drawer);
    printf("create_player inside %s\n", __func__);
    return NULL;
  }

  player_drawer->brush = create_brush(3, 20);
  if (player_drawer->brush == NULL) {
    destroy_player(player_drawer->player);
    free(player_drawer);
    printf("create_brush inside %s\n", __func__);
    return NULL;
  }
  player_drawer->player_state = player_type;

  player_drawer->cursor = POINTER;
  return player_drawer;
}

player_t *(player_drawer_get_player)(player_drawer_t *player_drawer) {
  return player_drawer->player;
}

void (player_drawer_set_cursor)(player_drawer_t *player_drawer, cursor_type_t cursor){
  player_drawer->cursor = cursor;
}

cursor_type_t (player_drawer_get_cursor)(player_drawer_t *player_drawer){
  return player_drawer->cursor;
}
