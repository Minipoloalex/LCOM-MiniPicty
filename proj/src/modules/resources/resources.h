#pragma once

#include <lcom/lcf.h>
#include "cursors.h"
#include "font.h"
#include "icons.h"
#include "asteroids.h"

#include "../../devices/video_card/video_card.h"
#include "../../model/sprite/sprite.h"

/** @defgroup resources Resources
 * @{
 *
 * @brief Module responsible for the resources (xpm images, prompts, icons, fonts, and cursors)
 */

/**
 * @brief Resources struct
 * Contains all the resources needed for the app
 */
typedef struct{
  Sprite* cursors[NUMBER_OF_CURSORS];
  Sprite* font[ALPHABET_SIZE]; 
  Sprite* icons[NUMBER_OF_ICONS];
  Sprite* asteroids[NUMBER_OF_ASTEROIDS];
} Resources;

/**
 * @brief Load all the resources by allocating memory and loading the xpm files into the sprites
 * 
 * @return Resources* - Reference to the resources struct 
 */
Resources *(load_resources)();

/**
 * @brief Unload all the resources by freeing the memory
 * 
 * @param resources - Reference to the resources struct
 */
void (unload_resources)(Resources *resources);
