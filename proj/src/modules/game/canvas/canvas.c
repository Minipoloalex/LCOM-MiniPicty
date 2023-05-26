#include "canvas.h"

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
  if (next.is_drawing && is_inside_rectangle(last, canvas->start_point, canvas->width, canvas->height)) {
    vg_draw_line(canvas->buffer, last, next.position, brush->size, brush->color);
  }
  return EXIT_SUCCESS;
}

uint8_t *(get_buffer)(canvas_t *canvas) {
  return canvas->buffer;
}

int (canvas_clear)(canvas_t *canvas) {
  memset(canvas->buffer, canvas->background_color, get_vram_size());
  return EXIT_SUCCESS;
}
