#pragma once
#include <lcom/lcf.h>
#include <stdbool.h>
#include "../brush/brush.h"
#include "../queue/queue.h"
#include "../position/position.h"
#include "../drawing_position.h"

typedef enum player_type {
  SELF_PLAYER, //desenha
  OTHER_PLAYER,//adivinha
} player_type_t;

struct Player;
typedef struct Player player_t;


player_t *(create_player)();
void (destroy_player)(player_t *player);

/**
 * @brief 
 * 
 */
int (player_add_next_position)(player_t *player, drawing_position_t *position);
/**
 * @brief 
 * @return int 0 if success, different than 0 if the queue is empty
 */
int (player_get_next_position)(player_t *player, drawing_position_t *position);
/**
 * @brief 
 * 
 */
int (player_get_last_position)(player_t *player, drawing_position_t *position);
/**
 * @brief 
 * 
 */
int (player_set_last_position)(player_t *player, drawing_position_t position);

/**
 * @brief returns the current position of the mouse of the player
 * That position is the last in the queue of positions. In case it is empty, returns the last position of the mouse already drawn.
 */
drawing_position_t (player_get_current_position)(player_t *player);



// PLAYER GUESSER STUFF
// struct PlayerGuesser;
// typedef struct PlayerGuesser PlayerGuesser_t;
// PlayerGuesser_t *(create_player_guesser)(player_type_t player_type);
// void (destroy_player_guesser)(PlayerGuesser_t *player_guesser);
// player_type_t (player_guesser_get_state)(PlayerGuesser_t *player_guesser);
