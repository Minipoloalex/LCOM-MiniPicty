#include "sprite.h"

Sprite * (create_sprite)(xpm_map_t xpm){
  Sprite *sprite = (Sprite *) malloc(sizeof(Sprite));
  if(sprite == NULL) return NULL;
  
  xpm_image_t loaded_xpm;
  sprite->colors = xpm_load(xpm, XPM_8_8_8_8, &loaded_xpm);
  
  if (sprite->colors == NULL || loaded_xpm.type == INVALID_XPM){
    printf("Error loading xpm\n");
    free(sprite);
    return NULL;
  }

  sprite->width = loaded_xpm.width;
  sprite->height = loaded_xpm.height;

  return sprite;
}

void (destroy_sprite)(Sprite *sprite){
  if(sprite == NULL) return;
  if(sprite->colors) free(sprite->colors);
  free(sprite);
}
