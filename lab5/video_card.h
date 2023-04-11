#include <lcom/lcf.h>
#include <stdint.h>

int (vg_enter)(uint16_t mode);
int (map_phys_mem_to_virtual)(uint16_t mode);
int (get_rgb_component)(uint32_t color, uint8_t component_size, uint8_t component_position, uint8_t *component);
// vg_draw_rectangle
// vg_draw_hline
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);
int (vg_erase_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);
int (update_position)(uint16_t *x, uint16_t xf, uint16_t *y, uint16_t yf, int16_t speed);
