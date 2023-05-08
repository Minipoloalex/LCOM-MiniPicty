#include "player.h"

struct PlayerDrawer {
  player_t *player;

  position_t last_mouse_position;
  queue_t *mouse_positions; // queue of drawing_position_t
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
  player_drawer->mouse_positions = create_queue(QUEUE_SIZE, sizeof(drawing_position_t));
  if (player_drawer->mouse_positions == NULL) {
    free(player_drawer);
    printf("create_queue inside %s\n", __func__);
    return NULL;
  }
  player_drawer->brush = create_brush(3, 20);
  if (player_drawer->brush == NULL) {
    free(player_drawer);
    free(player_drawer->mouse_positions);
    printf("create_brush inside %s\n", __func__);
    return NULL;
  }
  player_drawer->last_mouse_position = (position_t) {300, 300};
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
  destroy_brush(player_drawer->brush);
  delete_queue(player_drawer->mouse_positions);
  free(player_drawer);
}
void (destroy_player_guesser)(PlayerGuesser_t *player_guesser) {
  free(player_guesser);
}

int (player_add_next_position)(PlayerDrawer_t *player_drawer, drawing_position_t *position) {
  if (push_queue(player_drawer->mouse_positions, position) != OK) {
    printf("push_queue inside %s\n", __func__);
  }
  printf("Added this position to queue -> x: %d y: %d is_drawing: %d\n", position->position.x, position->position.y, position->is_drawing);
  return EXIT_SUCCESS;
}

int (player_get_next_position)(PlayerDrawer_t *player_drawer, drawing_position_t *position) {
  if (pop_queue(player_drawer->mouse_positions, position) != OK) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int (player_get_last_position)(PlayerDrawer_t *player_drawer, position_t *position) {
  *position = player_drawer->last_mouse_position;
  return EXIT_SUCCESS;
}
int (player_set_last_position)(PlayerDrawer_t *player_drawer, position_t position) {
  player_drawer->last_mouse_position = position;
  return EXIT_SUCCESS;
}
player_type_t (player_drawer_get_state)(PlayerDrawer_t *player_drawer) {
  return player_drawer->player_state;
}
player_type_t (player_guesser_get_state)(PlayerGuesser_t *player_guesser) {
  return player_guesser->player_state;
}
position_t (player_drawer_get_current_position)(PlayerDrawer_t *player_drawer) {
  drawing_position_t drawing_position;
  if (queue_get_back(player_drawer->mouse_positions, &drawing_position) != OK) {
    // queue is empty
    printf("queue is empty and return value is x: %d y: %d\n", player_drawer->last_mouse_position.x, player_drawer->last_mouse_position.y);
    return player_drawer->last_mouse_position;
  }
  printf("queue not empty and return value is x: %d y: %d, is_drawing: %d\n", drawing_position.position.x, drawing_position.position.y, drawing_position.is_drawing);
  return drawing_position.position;
}
brush_t *player_get_brush(PlayerDrawer_t *player_drawer) {
  return player_drawer->brush;
}
