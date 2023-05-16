#include "cursor.h"

int (draw_cursor)(cursor_image_t image, position_t pos){
  xpm_map_t cursor = cursors[image];
  //if (get_cursor_xpm(image, &cursor)) return EXIT_FAILURE;
  
  xpm_image_t cursor_image;
  uint8_t *colors = xpm_load(cursor, XPM_INDEXED, &cursor_image);
  if (colors == NULL || cursor_image.type == INVALID_XPM) return EXIT_FAILURE;
  cursor_image.bytes = colors;

  if (vg_draw_xpm(&cursor_image, pos.x, pos.y)) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
