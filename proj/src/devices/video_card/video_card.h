#include <lcom/lcf.h>
#include <stdint.h>
#include "../controllers/vbe.h"

/* Map physical memory to virtual address space of process */
int (map_phys_mem_to_virtual)(uint16_t mode);
/* Enter in Graphics or Text mode */
int (vg_enter)(uint16_t mode);
/* Get the color of a specific rgb component */
int (get_rgb_component)(uint32_t color, uint8_t component_size, uint8_t component_position, uint8_t *component);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);
int (vg_erase_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);
