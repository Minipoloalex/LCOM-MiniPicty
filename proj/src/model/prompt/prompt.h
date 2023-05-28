#include <stdlib.h>

#include <lcom/lcf.h>
#include "../../modules/resources/prompts.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../devices/serial_port/serial_port.h"

/** @defgroup prompt Prompt
 * @{
 *
 * @brief Module responsible for the word to guess (prompt)
 */

/**
 * @brief Generate a prompt by picking a random one from the prompts array
 * 
 * @param prompt - String to store the prompt
 * @return int - Index of the prompt chosen from the prompts array
 */
uint8_t (prompt_generate)(char *prompt);

/**
 * @brief Get the word from index object
 * 
 * @param index - Index of the prompt to get in the prompts array
 * @param prompt - String to store the prompt
 * @return int - Returns 0 upon success and non-zero otherwise
 */
int (get_word_from_index)(uint8_t index, char *prompt);
