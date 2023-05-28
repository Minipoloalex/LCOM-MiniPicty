#pragma once
#include "../../../model/player/player.h"

/** @defgroup player_menu PlayerMenu
 * @{
 *
 * @brief Module responsible for the player while on the menu screen
 */

typedef struct PlayerMenu player_menu_t;

/**
 * @brief Create a new Player Menu
 * 
 * @return player_menu_t* 
 */
player_menu_t *(create_player_menu)();

/**
 * @brief Destroy a Player Menu
 * 
 * @param player_menu 
 */
void (destroy_player_menu)(player_menu_t * player_menu);

/**
 * @brief Get the Player object
 * 
 * @param player_menu
 * @return reference to the player
 */
player_t *(player_menu_get_player)(player_menu_t * player_menu);
