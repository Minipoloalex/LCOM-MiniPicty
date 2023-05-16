#include "canvas.h"

canvas_t *(canvas_init)() {
  canvas_t *canvas = (canvas_t *) malloc(sizeof(canvas_t));
  // TODO: center canvas and limit drawing to inside canvas
  canvas->start_point.x = 200;
  canvas->start_point.y = 200;
  canvas->width = 400;
  canvas->height = 400;
  canvas->buffer = (uint8_t *) malloc(get_vram_size() * sizeof(uint8_t));
  memset(canvas->buffer, 0, get_vram_size());
  return canvas;
}

void (canvas_destroy)(canvas_t *canvas) {
  free(canvas->buffer);
  free(canvas);
}


int (draw_to_canvas)(canvas_t *canvas, player_drawer_t *player_drawer){
  if (canvas_draw_player_drawer(canvas, player_drawer)) {
    printf("canvas_draw_player_drawer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (canvas_draw_player_drawer)(canvas_t *canvas, player_drawer_t *player_drawer) {
  if (canvas == NULL) {
    printf("canvas is null\n");
    return EXIT_FAILURE;
  }
  if (player_drawer == NULL) {
    printf("player_drawer is null\n");
    return EXIT_FAILURE;
  }
  drawing_position_t drawing_position;
  position_t last_position;
  
  player_t *player = player_drawer_get_player(player_drawer);
  if (player == NULL) {
    printf("player is null\n");
    return EXIT_FAILURE;
  }
  brush_t *brush = player_drawer_get_brush(player_drawer);
  if (brush == NULL) {
    printf("brush is null\n");
    return EXIT_FAILURE;
  }

  while (player_get_next_position(player, &drawing_position) == OK) {
    set_needs_update(true);
    if (player_get_last_position(player, &last_position)) return EXIT_FAILURE;
    if (drawing_position.is_drawing) {
      vg_draw_line(canvas->buffer, last_position, drawing_position.position, brush->size, brush->color);
    }
    // printf("drawing position: %d %d %d\n", drawing_position.position.x, drawing_position.position.y, drawing_position.is_drawing);
    player_set_last_position(player, drawing_position.position);
  }
  return EXIT_SUCCESS;
}

uint8_t *(get_buffer)(canvas_t *canvas) {
  return canvas->buffer;
}
