#include "guess.h"

guess_word_t *(create_guess_word)(){
  guess_word_t *gwt = malloc(sizeof(gwt));
    if (gwt == NULL) {
      return NULL;
    }
    gwt->pointer = 0;
    return gwt;
}

int (write_character)(guess_word_t *gwt, uint8_t character){
  if (gwt->pointer >= 25) return EXIT_SUCCESS; //ignora caracter escrito
  
  gwt->string[gwt->pointer] = character;
  gwt->pointer++;

  return EXIT_SUCCESS;
}

int (validate_guess_word)(char *correct, guess_word_t *gwt, bool *right){
  if (strlen(correct) != gwt->pointer) {
    *right = false;
    return EXIT_SUCCESS;
  }

  for (size_t i = 0; i < strlen(correct); i++){
    if (gwt->string[i] != correct[i]) {
      *right = false;
      return EXIT_SUCCESS;
    }
  }

  *right = true;
  return EXIT_SUCCESS;
}

int (reset_guess_word)(guess_word_t *gwt){
  gwt->pointer = 0;
  return EXIT_SUCCESS;
}

int (delete_character)(guess_word_t *gwt){
  if (gwt->pointer <= 0) return EXIT_SUCCESS; //ignora o delete
  gwt->pointer--;
  return EXIT_SUCCESS;
}

void (destroy_guess_word)(guess_word_t *gwt){
  free(gwt);
}
