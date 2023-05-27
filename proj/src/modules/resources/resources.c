#include "resources.h"

Resources *(load_resources)(){
  Resources *resources = (Resources*) malloc(sizeof(Resources));
  if(resources == NULL) return NULL;
  for(size_t i = 0; i < NUMBER_OF_CURSORS; i++){
    resources->cursors[i] = create_sprite(cursors[i]);
  }

  for(size_t i=0; i<ALPHABET_SIZE; i++){
    resources->font[i] = create_sprite(uppercase_alphabet[i]);
  }

  for(size_t i=0; i<NUMBER_OF_ICONS; i++){
    resources->icons[i] = create_sprite(icons[i]);
  }

  for (size_t i = 0; i < NUMBER_OF_ASTEROIDS; i++) {
    resources->asteroids[i] = create_sprite(asteroids[i]);
  }
  return resources;
}

void (unload_resources)(Resources *resources){
  for(size_t i=0; i<NUMBER_OF_CURSORS; i++){
    destroy_sprite(resources->cursors[i]);
  }

  for(size_t i=0; i<ALPHABET_SIZE; i++){
    destroy_sprite(resources->font[i]);
  }

  for(size_t i=0; i<NUMBER_OF_ICONS; i++){
    destroy_sprite(resources->icons[i]);
  }

  for(size_t i=0; i<NUMBER_OF_ASTEROIDS; i++){
    destroy_sprite(resources->asteroids[i]);
  }

  free(resources);
}
