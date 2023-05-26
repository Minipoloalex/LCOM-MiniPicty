#include "prompt.h"

int (prompt_generate)(char *prompt){
  #ifndef RAND_INIT
  #define RAND_INIT
  srand(0);
  #endif

  int index = rand() % PROMPT_NUMBER;

  int i = 0;
  while (prompts[index][i] != 0){
    prompt[i] = prompts[index][i];
    i++;
  }
  prompt[i] = '\0';

  return EXIT_SUCCESS;
}
