#include "player.h"

struct Player {
  drawing_position_t last_mouse_position;
  queue_t *mouse_positions; // queue of drawing_position_t  
};

player_t *(create_player)() {
  player_t *player = malloc(sizeof(player_t));
  if (player == NULL) {
    return NULL;
  }
  player->mouse_positions = create_queue(QUEUE_SIZE, sizeof(drawing_position_t));
  if (player->mouse_positions == NULL) {
    free(player);
    printf("create_queue inside %s\n", __func__);
    return NULL;
  }
  player->last_mouse_position = (drawing_position_t) {{300, 300}, false};
  return player;
}

void (destroy_player)(player_t *player) {
  delete_queue(player->mouse_positions);
}

drawing_position_t (player_get_current_position)(player_t *player) {
  drawing_position_t drawing_position;
  if (is_empty_queue(player->mouse_positions)) {
    return player->last_mouse_position;
  }
  queue_get_back(player->mouse_positions, &drawing_position);
  return drawing_position;
}

int (player_add_next_position)(player_t *player, drawing_position_t *position) {
  if (push_queue(player->mouse_positions, position) != OK) {
    printf("push_queue inside %s\n", __func__);
  }
  return EXIT_SUCCESS;
}

int (player_get_next_position)(player_t *player, drawing_position_t *position) {
  if (pop_queue(player->mouse_positions, position) != OK) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int (player_get_last_position)(player_t *player, drawing_position_t *position) {
  *position = player->last_mouse_position;
  return EXIT_SUCCESS;
}
int (player_set_last_position)(player_t *player, drawing_position_t position) {
  player->last_mouse_position = position;
  return EXIT_SUCCESS;
}
