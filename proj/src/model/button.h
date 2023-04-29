#pragma once

#include <lcom/lcf.h>

struct button {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint8_t background_color;
    uint8_t text_color;
    char *text;
};
