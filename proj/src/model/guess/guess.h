#pragma once

#include "../../devices/keyboard/keyboard.h"

#define GUESS_LIMIT 15

typedef struct{
  char *string;
  size_t pointer;
} guess_word_t;

guess_word_t *(create_guess_word)();
int (write_character)(guess_word_t *gwt, uint8_t character);
int (delete_character)(guess_word_t *gwt);
int (validate_guess_word)(char *correct, guess_word_t *gwt, bool *right);
int (reset_guess_word)(guess_word_t *gwt);
void (destroy_guess_word)(guess_word_t *gwt);
