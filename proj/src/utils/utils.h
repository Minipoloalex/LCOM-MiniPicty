#pragma once

#include <lcom/lcf.h>
#include <stdint.h>

/** @defgroup utils Utils
 * @{
 *
 * @brief Module responsible for the utility functions used in the project
 */

/**
 * @brief Converts a byte to a string
 * 
 * @param byte byte to be converted
 * @return Pointer to string representation of the byte
 */
char *(byte_to_str)(uint8_t byte);

/**
 * @brief Returns the LSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param lsb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Returns the MSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param msb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 *
 * @param port the input port that is to be read
 * @param value a pointer to the variable that is to be update with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int (util_sys_inb)(int port, uint8_t *value);
