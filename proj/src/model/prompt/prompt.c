#include "prompt.h"

uint8_t (prompt_generate)(char *prompt){
  #ifndef RAND_INIT
  #define RAND_INIT
  srand(0);
  #endif

  int index = rand() % PROMPT_NUMBER;
  if (get_word_from_index(index, prompt) != OK) {
    printf("get_word failed inside %s\n", __func__);
    return EXIT_FAILURE;
  }  
  return index;
}

int (get_word_from_index)(uint8_t index, char *prompt) {
  if (index >= PROMPT_NUMBER) {
    printf("index out of bounds inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  int i = 0;
  while (prompts[index][i] != 0){
    prompt[i] = prompts[index][i];
    i++;
  }
  prompt[i] = '\0';
  return EXIT_SUCCESS;
}
