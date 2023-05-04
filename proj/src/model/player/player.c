#include "player.h"

struct PlayerDrawer {
  queue_t *mouse_positions;
  player_type_t player_state;
  brush_t *brush;
};
struct PlayerGuesser {
  position_t mouse_position;
  player_type_t player_state;
};

PlayerDrawer_t *(create_player_drawer)(player_type_t player_type) {
  PlayerDrawer_t *player_drawer = malloc(sizeof(PlayerDrawer_t));
  if (player_drawer == NULL) {
    return NULL;
  }
  player_drawer->player_state = player_type;
  player_drawer->mouse_positions = create_queue(QUEUE_SIZE, sizeof(position_t));
  player_drawer->brush = create_brush(10, 10, false);
  if (player_drawer->brush == NULL) {
    free(player_drawer);
    printf("create_brush inside %s\n", __func__);
    return NULL;
  }
  return player_drawer;
}
PlayerGuesser_t *(create_player_guesser)(player_type_t player_type) {
  PlayerGuesser_t *player_guesser = malloc(sizeof(PlayerGuesser_t));
  if (player_guesser == NULL) {
    return NULL;
  }
  player_guesser->player_state = player_type;
  player_guesser->mouse_position = (position_t) {0, 0};
  return player_guesser;
}
void (destroy_player_drawer)(PlayerDrawer_t *player_drawer) {
  free(player_drawer);
}
void (destroy_player_guesser)(PlayerGuesser_t *player_guesser) {
  free(player_guesser);
}

int (player_process_position)(PlayerDrawer_t *player_drawer, position_t position) {
  if (push_queue(player_drawer->mouse_positions, &position) != OK) {
    printf("push_queue inside %s\n", __func__);
  }
  return EXIT_SUCCESS;
}
