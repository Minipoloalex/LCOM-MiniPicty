#include "../../resources/cursors.h"
#include "../../devices/mouse/mouse.h"

typedef enum {POINTER, PEN, RUBBER} cursor_image_t;

int (draw_cursor)(cursor_image_t image, position_t pos);
