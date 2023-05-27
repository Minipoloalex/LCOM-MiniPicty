#pragma once

#include "../../devices/keyboard/keyboard.h"

#define GUESS_LIMIT 15

typedef struct{
  char *string;
  size_t pointer;
} guess_word_t;
/**
 * @brief 
 * 
 */
guess_word_t *(create_guess_word)();
/**
 * @brief 
 * 
 */
int (write_character)(guess_word_t *gwt, uint8_t character);
/**
 * @brief 
 * 
 */
int (delete_character)(guess_word_t *gwt);
/**
 * @brief 
 * 
 */
int (validate_guess_word)(char *correct, guess_word_t *gwt, bool *right);
/**
 * @brief 
 * 
 */
int (reset_guess_word)(guess_word_t *gwt);
/**
 * @brief 
 * 
 */
void (destroy_guess_word)(guess_word_t *gwt);
