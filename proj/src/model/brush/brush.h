#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct brush brush_t;
/*
TODO:
- Funções para aumentar/diminuir o tamanho do brush
- Função para mudar de cor do brush
*/
// int (brush_increase_size)(brush_t *brush);
// int (brush_decrease_size)(brush_t *brush);

bool (is_drawing)(brush_t *brush);
int (set_drawing)(brush_t *brush, bool drawing);
