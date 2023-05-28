#pragma once

#include "asteroids/meteor_ne.xpm"
#include "asteroids/meteor_nw.xpm"
#include "asteroids/meteor_se.xpm"
#include "asteroids/meteor_sw.xpm"

#define NUMBER_OF_ASTEROIDS 4 /**< @brief number of asteroid xpms */

/**
 * @brief Asteroids xpm array
 * Contains NUMBER_OF_ASTEROIDS xpms
 */
static xpm_map_t asteroids[NUMBER_OF_ASTEROIDS] = {
  meteor_ne_xpm,
  meteor_se_xpm,
  meteor_sw_xpm,
  meteor_nw_xpm
};

/**
 * @brief Asteroid directions types 
 * 
 */
typedef enum {UP_RIGHT, DOWN_RIGHT, DOWN_LEFT, UP_LEFT} asteroid_type_t;
