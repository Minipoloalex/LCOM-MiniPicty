#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>
#include "../controllers/vbe.h"
#include "../../model/position/position.h"
#include "../../modules/game/player_drawer/player_drawer.h"
#include "../../model/brush/brush.h"

/* Map physical memory to virtual address space of process */
int (map_phys_mem_to_virtual)(uint16_t mode);
/* Enter in Graphics or Text mode */
int (vg_enter)(uint16_t mode);
/* Get the color of a specific rgb component */
int (get_rgb_component)(uint32_t color, uint8_t component_size, uint8_t component_position, uint8_t *component);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_circle)(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
int (vg_draw_line)(position_t pos1, position_t pos2, uint16_t radius, uint32_t color);
int (vg_draw_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);
int (vg_erase_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);

int (vg_buffer_flip)();
int (vg_clear_buffer)(uint8_t buffer);

unsigned (get_vram_size)();
int (vg_copy_canvas_buffer)(uint8_t * buffer);


