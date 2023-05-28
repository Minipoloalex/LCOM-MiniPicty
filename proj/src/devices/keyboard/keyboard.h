#pragma once
#include <stdint.h>
#include <lcom/lcf.h>
#include "../controllers/i8042.h"
#include "../controllers/kbc/kbc.h"

/** @defgroup keyboard Keyboard
 * @{
 *
 * @brief Module responsible for the keyboard
 */

/**
 * @brief Struct that relates scancodes to their characters
 * 
 */
typedef struct Scancode {
    int scancode;
    char character;
} scancode_t;


/**
 * @brief Maps makecodes to their characters
 * 
 */
static scancode_t makecodes[36] = {
    {0x02, '1'},
    {0x03, '2'},
    {0x04, '3'},
    {0x05, '4'},
    {0x06, '5'},
    {0x07, '6'},
    {0x08, '7'},
    {0x09, '8'},
    {0x0A, '9'},
    {0x0B, '0'},
    {0x10, 'Q'},
    {0x11, 'W'},
    {0x12, 'E'},
    {0x13, 'R'},
    {0x14, 'T'},
    {0x15, 'Y'},
    {0x16, 'U'},
    {0x17, 'I'},
    {0x18, 'O'},
    {0x19, 'P'},
    {0x1E, 'A'},
    {0x1F, 'S'},
    {0x20, 'D'},
    {0x21, 'F'},
    {0x22, 'G'},
    {0x23, 'H'},
    {0x24, 'J'},
    {0x25, 'K'},
    {0x26, 'L'},
    {0x2C, 'Z'},
    {0x2D, 'X'},
    {0x2E, 'C'},
    {0x2F, 'V'},
    {0x30, 'B'},
    {0x31, 'N'},
    {0x32, 'M'},
};

/**
 * @brief Maps breakcodes to their characters
 * 
 */
static scancode_t breakcodes[36] = {
    {0x82, '1'},
    {0x83, '2'},
    {0x84, '3'},
    {0x85, '4'},
    {0x86, '5'},
    {0x87, '6'},
    {0x88, '7'},
    {0x89, '8'},
    {0x8A, '9'},
    {0x8B, '0'},
    {0x90, 'Q'},
    {0x91, 'W'},
    {0x92, 'E'},
    {0x93, 'R'},
    {0x94, 'T'},
    {0x95, 'Y'},
    {0x96, 'U'},
    {0x97, 'I'},
    {0x98, 'O'},
    {0x99, 'P'},
    {0x9E, 'A'},
    {0x9F, 'S'},
    {0xA0, 'D'},
    {0xA1, 'F'},
    {0xA2, 'G'},
    {0xA3, 'H'},
    {0xA4, 'J'},
    {0xA5, 'K'},
    {0xA6, 'L'},
    {0xAC, 'Z'},
    {0xAD, 'X'},
    {0xAE, 'C'},
    {0xAF, 'V'},
    {0xB0, 'B'},
    {0xB1, 'N'},
    {0xB2, 'M'},
};

/**
 * @brief Subscribes keyboard interrupts.
 * 
 * @param bit_no pointer to the variable that will store the bit mask returned upon an interrupt
 * @return 0 upon success, non-zero otherwise
 */
int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts.
 * 
 * @return 0 upon success, non-zero otherwise
 */
int (keyboard_unsubscribe_interrupts)();

/**
 * @brief Reads the KBC output and checks if it is valid
 */
void (keyboard_ih)();

/**
 * @brief Restores the KBC to its default state
 * 
 */
int (keyboard_restore)();

/**
 * @brief Checks if the scancode is a breakcode
 * 
 * @param scancode scancode to be verified
 * @param is_breakcode pointer to the variable that will store the result
 * 
 * @return 0 upon success, non-zero otherwise
 */
int (is_breakcode)(uint8_t scancode, bool *is_breakcode);

/**
 * @brief Translates a scancode to its character
 * 
 * @param scancode scancode to be translated
 * @param character pointer to the variable that will store the result
 * 
 * @return 0 upon success, non-zero otherwise
 */
int (translate_scancode)(uint8_t scancode, uint8_t *character);
