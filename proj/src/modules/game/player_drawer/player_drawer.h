#pragma once

#include "../../../model/player/player.h"
#include "../../../model/brush/brush.h"
#include "../../resources/cursors.h"

/** @defgroup player_drawer PlayerDrawer
 * @{
 *
 * @brief Module responsible for the drawing player
 */

struct PlayerDrawer;
typedef struct PlayerDrawer player_drawer_t;

/**
 * @brief Create a new Player Drawer
 * 
 * @param player_type 
 * @return player_drawer_t* 
 */
player_drawer_t *(create_player_drawer)(player_type_t player_type);

/**
 * @brief Destroy a Player Drawer
 * 
 * @param player_drawer 
 */
void (destroy_player_drawer)(player_drawer_t *player_drawer);

/**
 * @brief Get the Brush object from the player
 * 
 * @param player_drawer 
 */
brush_t *player_drawer_get_brush(player_drawer_t *player_drawer);

/**
 * @brief Get the Player type object
 * 
 * @param player_drawer A reference to the player drawer
 * @return player_t* 
 */
player_type_t (player_drawer_get_role)(player_drawer_t *player_drawer);

/**
 * @brief Change the player type
 * 
 * @param player_drawer A reference to the player drawer
 */
int (player_drawer_change_role)(player_drawer_t *player_drawer);

/**
 * @brief Get the Player object
 * 
 * @param player_drawer A reference to the player drawer
 */
player_t *(player_drawer_get_player)(player_drawer_t *player_drawer);

/**
 * @brief Set the Player Cursor object to the player
 * 
 * @param player_drawer A reference to the player drawer
 * @param cursor The cursor to be set
 */
void (player_drawer_set_cursor)(player_drawer_t *player_drawer, cursor_type_t cursor);

/**
 * @brief Get the Player Cursor type from the player
 * 
 * @param player_drawer A reference to the player drawer
 * @return cursor_type_t 
 */
cursor_type_t (player_drawer_get_cursor)(player_drawer_t *player_drawer);
