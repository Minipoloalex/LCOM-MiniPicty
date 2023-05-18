#include "prompt.h"

int (prompt_generate)(char *prompt){
  #ifndef RAND_INIT
  #define RAND_INIT
  srand(0);
  #endif

  int index = rand() % PROMPT_NUMBER;

  prompt = malloc(strlen(prompts[index]) + 1);
  if (prompt == NULL) return EXIT_FAILURE;
  strcpy(prompt, prompts[index]);

  printf("defined prompt: %s\n", prompt);
  return EXIT_SUCCESS;
}

int (prompt_destroy)(char *prompt){
  free(prompt);
  return EXIT_SUCCESS;
}
