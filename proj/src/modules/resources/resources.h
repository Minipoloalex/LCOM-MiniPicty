#pragma once

#include <lcom/lcf.h>
#include "cursors.h"
#include "font.h"
#include "icons.h"
#include "../../model/sprite/sprite.h"

typedef struct{
  Sprite* cursors[NUMBER_OF_CURSORS];
  Sprite* font[ALPHABET_SIZE]; 
  Sprite* icons[NUMBER_OF_ICONS]; 
} Resources;

Resources *(load_resources)();

void (unload_resources)(Resources *resources);
