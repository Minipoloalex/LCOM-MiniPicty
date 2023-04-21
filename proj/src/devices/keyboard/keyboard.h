#pragma once
#include <stdint.h>
#include <lcom/lcf.h>
#include "../controllers/i8042.h"
#include "../controllers/kbc/kbc.h"

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);
int (keyboard_unsubscribe_interrupts)();
void (keyboard_ih)();
int (keyboard_restore)();

struct ScancodeMap {
    int scancode;
    char character;
};

struct ScancodeMap breakcodes[] = {
    {0x81,  'ESC'},
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




