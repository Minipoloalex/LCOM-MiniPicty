#pragma once

#include <lcom/lcf.h>

#include "cursors/cursor.xpm"
#include "cursors/pen.xpm"
#include "cursors/rubber.xpm"

#define NUMBER_OF_CURSORS 3 /**< @brief number of cursor xpms */

/**
 * @brief Cursors xpm array
 * Contains NUMBER_OF_CURSORS xpms
 */
static xpm_map_t cursors[NUMBER_OF_CURSORS] = {
  cursor_xpm,
  pen_xpm,
  rubber_xpm
};

/**
 * @brief Cursors types
 * 
 */
typedef enum {POINTER, PEN, RUBBER} cursor_type_t;
