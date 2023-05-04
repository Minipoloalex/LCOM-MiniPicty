#pragma once
#include <lcom/lcf.h>
#include "../brush/brush.h"
#include "../queue/queue.h"
#include "../position/position.h"

typedef enum player_type {
  SELF_PLAYER,
  OTHER_PLAYER,
} player_type_t;


typedef struct PlayerDrawer PlayerDrawer_t;
typedef struct PlayerGuesser PlayerGuesser_t;

PlayerDrawer_t *(create_player_drawer)(player_type_t player_type);
PlayerGuesser_t *(create_player_guesser)(player_type_t player_type);
void (destroy_player_drawer)(PlayerDrawer_t *player_drawer);
void (destroy_player_guesser)(PlayerGuesser_t *player_guesser);
int (player_process_position)(PlayerDrawer_t *player_drawer, position_t position);
