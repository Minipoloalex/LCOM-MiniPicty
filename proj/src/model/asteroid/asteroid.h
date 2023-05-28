#pragma once
#include <lcom/lcf.h>

#include "../position/position.h"
#include "../../devices/video_card/video_card.h"
#include "../../modules/resources/asteroids.h"

#define ASTEROID_INITIAL_X_SPEED 1  /**< @brief Initial x speed of the asteroid */
#define ASTEROID_INITIAL_Y_SPEED 1 /**< @brief Initial y speed of the asteroid */
#define ASTEROID_INITIAL_XPM DOWN_RIGHT /**< @brief Initial direction of the asteroid */

/** @defgroup asteroid Asteroid
 * @{
 *
 * @brief Module responsible for the asteroid
 */

/**
 * @brief Asteroid struct.
 * Has a position, x and y speed, an array of xpms and the current xpm.
 */
typedef struct {
  position_t position;
  int8_t x_speed;
  int8_t y_speed;

  Sprite **xpms;
  asteroid_type_t current_xpm;
} asteroid_t;

/**
 * @brief Creates an asteroid.
 * 
 * @param xpms array of XPM's to be used by the asteroid
 * @return asteroid_t* pointer to the created asteroid
 */
asteroid_t *(create_asteroid)(Sprite *xpms[]);
/**
 * @brief Destroys an asteroid
 * @param asteroid asteroid to be destroyed
 */
void (destroy_asteroid)(asteroid_t *asteroid);
/**
 * @brief Checks if a position collides with an asteroid
 * This is used for checking if the mouse is inside the asteroid to prevent drawing.
 * The algorithm used for this is pixel-perfect-collision.
 */
bool (is_inside_asteroid)(asteroid_t *asteroid, position_t position);
/**
 * @brief Draws an asteroid with its current xpm at its current position.
 * 
 * @return 0 upon success, 1 otherwise
 */
int (draw_asteroid)(asteroid_t *asteroid);
/**
 * @brief Updates the asteroid's position, speed and xpms to be the default initial values.
 */
void (asteroid_reset_position)(asteroid_t *asteroid);
