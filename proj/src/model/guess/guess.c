#include "guess.h"

guess_word_t *(create_guess_word)(){
  guess_word_t *gwt = (guess_word_t *) malloc(sizeof(guess_word_t));
    if (gwt == NULL) {
      printf("malloc inside %s\n", __func__);
      return NULL;
    }
    gwt->string = (char *) malloc(GUESS_LIMIT * sizeof(char));
    if (gwt->string == NULL) {
      printf("malloc inside %s\n", __func__);
      free(gwt);
      return NULL;
    }
    gwt->pointer = 0;
    return gwt;
}

int (write_character)(guess_word_t *gwt, uint8_t character){
  if (gwt->pointer >= GUESS_LIMIT) return EXIT_SUCCESS; //ignora caracter escrito
  
  gwt->string[gwt->pointer] = character;
  gwt->pointer++;

  //if (gwt->pointer < GUESS_LIMIT - 1) gwt->string[gwt->pointer+1] = '\0';
  return EXIT_SUCCESS;
}

int (validate_guess_word)(char *correct, guess_word_t *gwt, bool *right){
  //! for debug
  /*
  printf("correct size: %d, guess size: %d\n", strlen(correct), gwt->pointer);
  printf("correct: %s\n", correct);
  
  printf("guess: ");
  for (size_t i = 0; i < gwt->pointer; i++){
    printf("%c", gwt->string[i]);
  }
  printf("\n");
  */
  
  //check size
  if (strlen(correct) != (gwt->pointer)) {
    *right = false;
    return EXIT_SUCCESS;
  }

  //check content
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
  free(gwt->string);
  free(gwt);
}
