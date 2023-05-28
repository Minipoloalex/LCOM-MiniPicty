#pragma once

#include "../../devices/keyboard/keyboard.h"

#define GUESS_LIMIT 15  /**< @brief maximum number of characters in the guess word */

/** @defgroup guess Guess
 * @{
 *
 * @brief Module responsible for the guess word
 */

/**
 * @brief Guess word struct
 * 
 */
typedef struct{
  char *string;
  size_t pointer;
} guess_word_t;

/**
 * @brief Create a new Guess Word
 * 
 * @return guess_word_t* pointer to the new Guess Word
 */
guess_word_t *(create_guess_word)();
/**
 * @brief Destroy a Guess Word
 * 
 * @param gwt Guess Word to destroy
 */
void (destroy_guess_word)(guess_word_t *gwt);

/**
 * @brief Insert a character in the guess word
 * 
 * @param gwt Guess Word to insert the character
 * @param character character to insert
 * @return int returns 0 upon success and non-zero otherwise
 */
int (write_character)(guess_word_t *gwt, uint8_t character);
/**
 * @brief Delete the last character of the guess word if the length of the word is greater than 0
 * 
 * @param gwt Guess Word to delete the character
 * @return int returns 0 upon success and non-zero otherwise
 */
int (delete_character)(guess_word_t *gwt);

/**
 * @brief Validate the guess word by comparing it to the correct word and setting the right flag to true if it's correct and false otherwise
 * 
 * @param correct the correct word to compare to
 * @param gwt word to validate
 * @param right result of the validation
 * @return int returns 0 upon success and non-zero otherwise
 */
int (validate_guess_word)(char *correct, guess_word_t *gwt, bool *right);

/**
 * @brief Reset the guess word
 * 
 * @param gwt guess word to reset
 * @return int returns 0 upon success and non-zero otherwise
 */
int (reset_guess_word)(guess_word_t *gwt);
