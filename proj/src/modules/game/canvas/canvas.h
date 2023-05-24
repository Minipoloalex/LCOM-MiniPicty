#pragma once
#include <lcom/lcf.h>
#include "../../../model/position/position.h"
#include "../../../model/brush/brush.h"
#include "../../../devices/video_card/video_card.h"

#include "../player_drawer/player_drawer.h"


/** @defgroup canvas Canvas
 * @{
 *
 * @brief Module responsible for the canvas
 */


/**
 * @brief 
 * 
 */
typedef struct {
  position_t start_point; /**< @brief position of the canvas */
  uint16_t width; /**< @brief width of the canvas */
  uint16_t height; /**< @brief height of the canvas */
  uint8_t *buffer; /**< @brief buffer of the canvas */
  int background_color; /**< @brief background color of the canvas */
} canvas_t;

/**
 * @brief Initializes a canvas
 * 
 * @param x x position of the canvas
 * @param y y position of the canvas
 * @param width width of the canvas
 * @param height height of the canvas
 * @return canvas_t* pointer to the canvas
 */
canvas_t *(canvas_init)(int x, int y, int width, int height);
void (canvas_destroy)(canvas_t *canvas);
int (draw_to_canvas)(canvas_t *canvas, player_drawer_t *player_drawer);
int (canvas_draw_player_drawer)(canvas_t *canvas, player_drawer_t *player_drawer);
uint8_t *(get_buffer)(canvas_t *canvas);
int (canvas_clear)(canvas_t *canvas);

/** @} end of canvas */
