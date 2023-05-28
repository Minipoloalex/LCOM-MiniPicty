#pragma once

#include <lcom/lcf.h>

#include "icons/plus.xpm"
#include "icons/minus.xpm"
#include "cursors/rubber.xpm"
#include "icons/trash.xpm"

#define NUMBER_OF_ICONS 4 /**< @brief number of icon xpms */

/**
 * @brief Icons xpm array
 * Contains NUMBER_OF_ICONS xpms
 */
static xpm_map_t icons[NUMBER_OF_ICONS] = {
  plus_xpm,
  minus_xpm,
  rubber_xpm,
  trash_xpm,
};
