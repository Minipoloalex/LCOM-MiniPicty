#include "player.h"

struct Player {
  position_t last_mouse_position;
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
  player->last_mouse_position = (position_t) {300, 300};
  return player;
}

void (destroy_player)(player_t *player) {
  delete_queue(player->mouse_positions);
}


position_t (player_get_current_position)(player_t *player) {
  drawing_position_t drawing_position;
  if (queue_get_back(player->mouse_positions, &drawing_position) != OK) {
    // queue is empty
    printf("queue is empty and return value is x: %d y: %d\n", player->last_mouse_position.x, player->last_mouse_position.y);
    return player->last_mouse_position;
  }
  printf("queue not empty and return value is x: %d y: %d, is_drawing: %d\n", drawing_position.position.x, drawing_position.position.y, drawing_position.is_drawing);
  return drawing_position.position;
}

int (player_add_next_position)(player_t *player, drawing_position_t *position) {
  if (push_queue(player->mouse_positions, position) != OK) {
    printf("push_queue inside %s\n", __func__);
  }
  printf("Added this position to queue -> x: %d y: %d is_drawing: %d\n", position->position.x, position->position.y, position->is_drawing);
  return EXIT_SUCCESS;
}

int (player_get_next_position)(player_t *player, drawing_position_t *position) {
  if (pop_queue(player->mouse_positions, position) != OK) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int (player_get_last_position)(player_t *player, position_t *position) {
  *position = player->last_mouse_position;
  return EXIT_SUCCESS;
}
int (player_set_last_position)(player_t *player, position_t position) {
  player->last_mouse_position = position;
  return EXIT_SUCCESS;
}



// Player guesser stuff 
/*
struct PlayerGuesser {
  position_t mouse_position;
  player_type_t player_state;
};
player_type_t (player_guesser_get_state)(PlayerGuesser_t *player_guesser) {
  return player_guesser->player_state;
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

void (destroy_player_guesser)(PlayerGuesser_t *player_guesser) {
  free(player_guesser);
}
*/
