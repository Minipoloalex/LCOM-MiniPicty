#ifndef CURSOR_H
#define CURSOR_H

#include <lcom/lcf.h>

#include "cursors/cursor.xpm"
#include "cursors/pen.xpm"
#include "cursors/rubber.xpm"

#define NUMBER_OF_CURSORS 3

static xpm_map_t cursors[NUMBER_OF_CURSORS] = {
  cursor_xpm,
  pen_xpm,
  rubber_xpm
};

typedef enum {POINTER, PEN, RUBBER} cursor_type_t;

#endif
