#include <lcom/lcf.h>
#include <stdint.h>

int (vg_enter)(uint16_t mode);
int (map_phys_mem_to_virtual)(uint16_t mode);
int (get_rgb_component)(uint32_t color, uint8_t component_size, uint8_t component_position, uint8_t *component);
// vg_draw_rectangle
// vg_draw_hline
// vg_draw_pixel
