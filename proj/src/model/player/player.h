#pragma once
#include <lcom/lcf.h>
#include <stdbool.h>
#include "../brush/brush.h"
#include "../queue/queue.h"
#include "../position/position.h"

/** @defgroup player Player
 * @{
 *
 * @brief Module responsible for the player and its position on the screen
 */

typedef enum player_type {
  SELF_PLAYER,   /**< @brief Player that draws */
  OTHER_PLAYER, /**< @brief Player that guesses */
} player_type_t;

struct Player;
typedef struct Player player_t;

/**
 * @brief Create a new Player
 * 
 * @return Pointer to created player
 */
player_t *(create_player)();
/**
 * @brief Destroy a Player
 * 
 * @param player player to be destroyed
 */
void (destroy_player)(player_t *player);

/**
 * @brief Add a new position to the queue of positions of the player
 * 
 * @param player player to add the position
 * @param position position to be added
 * @return int returs 0 upon success and non-zero otherwise
 */
int (player_add_next_position)(player_t *player, drawing_position_t *position);
/**
 * @brief Get the next position of the queue of positions of the player
 * 
 * @param player player to get the position
 * @param position position to be returned
 * @return int returns 0 upon success and non-zero otherwise
 */
int (player_get_next_position)(player_t *player, drawing_position_t *position);
/**
 * @brief Get the last position assigned to the player
 * 
 * @param player player to get the position
 * @param position position to be returned
 * @return int returns 0 upon success and non-zero otherwise
 */
int (player_get_last_position)(player_t *player, drawing_position_t *position);
/**
 * @brief Set the last position in the player
 * 
 * @param player player to set the position
 * @param position position to be set
 * @return int returns 0 upon success and non-zero otherwise
 */
int (player_set_last_position)(player_t *player, drawing_position_t position);

/**
 * @brief Get the last position of the queue of positions of the player.
 * In case it is empty, returns the last position of the mouse already drawn.
 * 
 * @param player player to set the position
 * @return returns the current position of the mouse of the player
 */
drawing_position_t (player_get_current_position)(player_t *player);
