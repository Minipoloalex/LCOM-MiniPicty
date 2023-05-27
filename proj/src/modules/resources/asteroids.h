#pragma once

#include "asteroids/asteroid_ne.xpm"
#include "asteroids/asteroid_nw.xpm"
#include "asteroids/asteroid_se.xpm"
#include "asteroids/asteroid_sw.xpm"

#define NUMBER_OF_ASTEROIDS 4 /**< @brief number of asteroid xpms */

/**
 * @brief Array of asteroid xpms.
 * Contains NUMBER_OF_ASTEROIDS xpms.
 */
static xpm_map_t asteroids[NUMBER_OF_ASTEROIDS] = {
  asteroid_ne_xpm,
  asteroid_se_xpm,
  asteroid_sw_xpm,
  asteroid_nw_xpm
};
/**
 * @brief enum with asteroid directions
 */
typedef enum {UP_RIGHT, DOWN_RIGHT, DOWN_LEFT, UP_LEFT} asteroid_type_t;
