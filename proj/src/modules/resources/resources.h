#pragma once

#include <lcom/lcf.h>
#include "cursors.h"
#include "font.h"
#include "icons.h"
#include "asteroids.h"

#include "../../devices/video_card/video_card.h"
#include "../../model/sprite/sprite.h"

typedef struct{
  Sprite* cursors[NUMBER_OF_CURSORS];
  Sprite* font[ALPHABET_SIZE]; 
  Sprite* icons[NUMBER_OF_ICONS];
  Sprite* asteroids[NUMBER_OF_ASTEROIDS];
} Resources;

Resources *(load_resources)();

void (unload_resources)(Resources *resources);
