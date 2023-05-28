#include "canvas.h"

/**
 * @brief Bresenham's algorithm for drawing lines between two points
 * @param pos1 position of the first point
 * @param pos2 position of the second point
 * @param radius radius of the line
 * @param color color of the line
 * @param asteroid asteroid to check if its possible to draw
 */
int (canvas_draw_line)(uint8_t *buffer, position_t pos1, position_t pos2, uint16_t thickness, uint32_t color, asteroid_t *asteroid);

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

int (draw_in_canvas)(canvas_t *canvas, brush_t *brush, position_t last, drawing_position_t next, asteroid_t *asteroid) {
  if (next.is_drawing && is_inside_rectangle(last, canvas->start_point, canvas->width, canvas->height)) {
    canvas_draw_line(canvas->buffer, last, next.position, brush->size, brush->color, asteroid);
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

int (canvas_draw_line)(uint8_t *buffer, position_t pos1, position_t pos2, uint16_t thickness, uint32_t color, asteroid_t *asteroid) {
    int x0 = pos1.x;
    int y0 = pos1.y;
    int x1 = pos2.x;
    int y1 = pos2.y;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    int e2;
    // draw first point of the line with specified color
    if (asteroid == NULL || !is_inside_asteroid(asteroid, pos1)) {
      vg_draw_circle_to_buffer(buffer, x0, y0, thickness / 2, color);
    }
    
    while (x0 != x1 || y0 != y1) {
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }

        if (asteroid == NULL || !is_inside_asteroid(asteroid, (position_t) {.x = x0, .y = y0})) {
          vg_draw_circle_to_buffer(buffer, x0, y0, thickness / 2, color);
        }
    }

    if (asteroid == NULL || !is_inside_asteroid(asteroid, pos2)) {
      vg_draw_circle_to_buffer(buffer, x1, y1, thickness / 2, color);
    }

    return EXIT_SUCCESS;
}
