#pragma once
#include <lcom/lcf.h>
#include "../../../model/position/position.h"
#include "../../../model/brush/brush.h"
#include "../../../devices/video_card/video_card.h"

#include "../player_drawer/player_drawer.h"
#include "../../../model/asteroid/asteroid.h"

#define CANVAS_BACKGROUND_COLOR 0XFFFFFF /**< @brief background color of the canvas */

/** @defgroup canvas Canvas
 * @{
 *
 * @brief Module responsible for the canvas
 */

/**
 * @brief canvas struct
 * 
 */
typedef struct {
  position_t start_point; /**< @brief position of the canvas */
  uint16_t width; /**< @brief width of the canvas */
  uint16_t height; /**< @brief height of the canvas */
  uint8_t *buffer; /**< @brief buffer of the canvas */
  uint32_t background_color; /**< @brief background color of the canvas */
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

/**
 * @brief Destroys a canvas, by freeing its memory
 * 
 * @param canvas canvas to be destroyed
 */
void (canvas_destroy)(canvas_t *canvas);

/**
 * @brief Draws a line in the canvas
 * 
 * @param canvas canvas to draw the line
 * @param brush brush to draw the line
 * @param last last position of the line
 * @param next next position of the line
 * @param asteroid asteroid to check if its possible to draw
 * @return int 0 if success, 1 otherwise
 */
int (draw_in_canvas)(canvas_t *canvas, brush_t *brush, position_t last, drawing_position_t next, asteroid_t *asteroid);

/**
 * @brief Gets the buffer of the canvas
 * 
 * @param canvas canvas to get the buffer
 * @return uint8_t* pointer to the buffer
 */
uint8_t *(get_buffer)(canvas_t *canvas);

/**
 * @brief Clears the canvas
 * 
 * @param canvas canvas to be cleared
 * @return int 0 if success, 1 otherwise
 */
int (canvas_clear)(canvas_t *canvas);

/** @} end of canvas */
