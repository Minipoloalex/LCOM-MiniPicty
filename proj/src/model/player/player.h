#pragma once
#include <lcom/lcf.h>
#include <stdbool.h>
#include "../brush/brush.h"
#include "../queue/queue.h"
#include "../position/position.h"
#include "../drawing_position.h"

typedef enum player_type {
  SELF_PLAYER,
  OTHER_PLAYER,
} player_type_t;

struct PlayerDrawer;
struct PlayerGuesser;
typedef struct PlayerDrawer PlayerDrawer_t;
typedef struct PlayerGuesser PlayerGuesser_t;

/**
 * @brief 
 * 
 */
PlayerDrawer_t *(create_player_drawer)(player_type_t player_type);
/**
 * @brief 
 * 
 */
PlayerGuesser_t *(create_player_guesser)(player_type_t player_type);
/**
 * @brief 
 * 
 */
void (destroy_player_drawer)(PlayerDrawer_t *player_drawer);
/**
 * @brief 
 * 
 */
void (destroy_player_guesser)(PlayerGuesser_t *player_guesser);
/**
 * @brief 
 * 
 */
int (player_add_next_position)(PlayerDrawer_t *player_drawer, drawing_position_t *position);

/**
 * @brief 
 * @return int 0 if success, different than 0 if the queue is empty
 */
int (player_get_next_position)(PlayerDrawer_t *player_drawer, drawing_position_t *position);

