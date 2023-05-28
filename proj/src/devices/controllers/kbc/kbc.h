#pragma once
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../i8042.h"

/** @defgroup kbc KBC
 * @{
 *
 * @brief Module responsible for the kbc
 */

/**
 * @brief Reads the KBC output
 * @param port port to read from
 * @param output pointer to the variable that will store the output
 * @param mouse true if the output is from the mouse, false otherwise
 * @return 0 upon success, non-zero otherwise
 */
int (read_KBC_output)(int8_t port, uint8_t* output, uint8_t mouse);

/**
 * @brief Writes a command to the KBC
 * 
 * @param port port to write to
 * @param cmd_byte command to write
 * 
 * @return 0 upon success, non-zero otherwise
 */
int (write_KBC_command)(uint8_t port, uint8_t cmd_byte);

/**
 * @brief Writes a command to the mouse
 * 
 * @param command command to write
 * 
 * @return 0 upon success, non-zero otherwise
 */
int (write_to_mouse)(uint8_t command);

/**
 * @brief Reads KBC output but doesn't do anything with it
 * @return 0 upon success, non-zero otherwise
 */
int (read_KBC_output_to_trash)();
