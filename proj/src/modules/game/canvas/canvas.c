#include "canvas.h"

#define CANVAS_BACKGROUND_COLOR 63

canvas_t *(canvas_init)(int x, int y, int width, int height) {
  canvas_t *canvas = (canvas_t *) malloc(sizeof(canvas_t));
  canvas->start_point.x = x;
  canvas->start_point.y = y;
  canvas->width = width;
  canvas->height = height;
  canvas->buffer = (uint8_t *) malloc(get_vram_size() * sizeof(uint8_t));
  canvas->background_color = CANVAS_BACKGROUND_COLOR;
  memset(canvas->buffer, canvas->background_color, get_vram_size());
  return canvas;
}

void (canvas_destroy)(canvas_t *canvas) {
  free(canvas->buffer);
  free(canvas);
}

int (draw_in_canvas)(canvas_t *canvas, brush_t *brush, position_t last, drawing_position_t next) {
  if (next.is_drawing && canvas_contains_position(canvas, last)) {
    vg_draw_line(canvas->buffer, last, next.position, brush->size, brush->color);
  }
  return EXIT_SUCCESS;
}

// int (canvas_draw_player_drawer)(canvas_t *canvas, player_drawer_t *player_drawer) {
//   if (canvas == NULL) {
//     printf("canvas is null\n");
//     return EXIT_FAILURE;
//   }
//   if (player_drawer == NULL) {
//     printf("player_drawer is null\n");
//     return EXIT_FAILURE;
//   }
//   drawing_position_t drawing_position;
//   drawing_position_t last_position;
  
//   player_t *player = player_drawer_get_player(player_drawer);
//   if (player == NULL) {
//     printf("player is null\n");
//     return EXIT_FAILURE;
//   }
//   brush_t *brush = player_drawer_get_brush(player_drawer);
//   if (brush == NULL) {
//     printf("brush is null\n");
//     return EXIT_FAILURE;
//   }

//   while (player_get_next_position(player, &drawing_position) == OK) {
//     set_needs_update(true);
//     if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;
//     bool position_is_inside_canvas = drawing_position.position.x >= canvas->start_point.x && drawing_position.position.x < canvas->start_point.x + canvas->width && drawing_position.position.y >= canvas->start_point.y && drawing_position.position.y < canvas->start_point.y + canvas->height;
//     if (drawing_position.is_drawing && position_is_inside_canvas) {
//       vg_draw_line(canvas->buffer, last_position.position, drawing_position.position, brush->size, brush->color);
//     }
//     player_set_last_position(player, drawing_position);
//   }
//   return EXIT_SUCCESS;
// }

uint8_t *(get_buffer)(canvas_t *canvas) {
  return canvas->buffer;
}

int (canvas_clear)(canvas_t *canvas) {
  memset(canvas->buffer, canvas->background_color, get_vram_size());
  return EXIT_SUCCESS;
}

bool (canvas_contains_position)(canvas_t *canvas, position_t position){
  return (
    (canvas->start_point.x <= position.x) && (position.x < (canvas->start_point.x+canvas->width))&&
    (canvas->start_point.y <= position.y) && (position.y < (canvas->start_point.y+canvas->height))
  );
}
