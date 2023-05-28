#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#define MIN_BRUSH_SIZE 1  /**< @brief minimum size of the circles drawn by the brush */
#define MAX_BRUSH_SIZE 35  /**< @brief maximum size of the circles drawn by the brush*/
#define BRUSH_STEP 3 /**< @brief step of the brush size when increasing or decreasing it */ 

/** @defgroup brush Brush
 * @{
 *
 * @brief Module responsible for the brush
 */

/**
 * @brief Brush struct. Has a color and size.
 */
struct brush {
  uint32_t color;
  uint16_t size;
};
typedef struct brush brush_t;
/**
 * @brief Creates a brush with a given color and size.
 * @param color color of the created brush
 * @param size size of the created brush
 * @return brush_t* pointer to the created brush
 */
brush_t *(create_brush)(uint32_t color, uint16_t size);
/**
 * @brief Destroys a brush.
 * 
 * @param brush brush to be destroyed
 */
void (destroy_brush)(brush_t *brush);
/**
 * @brief Sets the color of the brush.
 * 
 * @param brush brush to change its color
 * @param color new color of the brush
 */
void (set_brush_color)(brush_t *brush, uint32_t color);
/**
 * @brief Increases the size of a brush by BRUSH_STEP pixels
 * 
 * @param brush brush to increase its size
 */
void (brush_increase_size)(brush_t *brush);
/**
 * @brief Decreases the size of a brush by BRUSH_STEP pixels
 * 
 * @param brush brush to decrease its size
 */
void (brush_decrease_size)(brush_t *brush);
