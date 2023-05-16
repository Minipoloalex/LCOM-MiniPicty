#include "player_drawer.h"

struct PlayerDrawer {
  player_t *player;

  player_type_t player_state;
  brush_t *brush;
};

void (destroy_player_drawer)(player_drawer_t *player_drawer) {
  destroy_brush(player_drawer->brush);
  destroy_player(player_drawer->player);
  free(player_drawer);
}

brush_t *player_drawer_get_brush(player_drawer_t *player_drawer) {
  return player_drawer->brush;
}

player_type_t (player_drawer_get_state)(player_drawer_t *player_drawer) {
  return player_drawer->player_state;
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
  return player_drawer;
}

player_t *(player_drawer_get_player)(player_drawer_t *player_drawer) {
  return player_drawer->player;
}
