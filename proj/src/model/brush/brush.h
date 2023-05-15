#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

struct brush {
  uint16_t color;
  uint16_t size;
};
typedef struct brush brush_t;
/*
TODO:
- Funções para aumentar/diminuir o tamanho do brush
- Função para mudar de cor do brush
*/
// int (brush_increase_size)(brush_t *brush);
// int (brush_decrease_size)(brush_t *brush);

brush_t *(create_brush)(uint16_t color, uint16_t size);
void (destroy_brush)(brush_t *brush);
