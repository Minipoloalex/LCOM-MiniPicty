#ifndef CURSOR_H
#define CURSOR_H

#include <lcom/lcf.h>

#include "cursors/cursor.xpm"
#include "cursors/pen.xpm"
#include "cursors/rubber.xpm"

static xpm_map_t cursors[3] = {
  cursor_xpm,
  pen_xpm,
  rubber_xpm
};

#endif
