#include <lcom/lcf.h>
#include <stdint.h>
#include <math.h>
#include "../controllers/vbe.h"
#include "../../resources/font.h"
#include "../../model/button/button.h"
#include "../../model/position/position.h"
#include "../../modules/game/player_drawer/player_drawer.h"
#include "../../model/brush/brush.h"
#include "../../model/guess/guess.h"
#include "../../model/cursor/cursor.h"

//guess word
#define GUESS_POS_X 30
#define GUESS_POS_Y 800

//guess box
#define GUESS_BOX_X 15
#define GUESS_BOX_Y 780
#define GUESS_BOX_WIDTH 1024-30 //hres-30
#define GUESS_BOX_HEIGHT 40+FONT_HEIGHT

//round timer
#define ROUND_TIMER_X 900
#define ROUND_TIMER_Y 700

//colors
#define TRANSPARENT 0x000001
#define BLACK 0X000000
#define WHITE 0XFFFFFF

// Buffer related functions
bool (buffers_need_update)();
void (set_needs_update)(bool value);

int (setup_video_mode)(uint16_t mode);
/* Map physical memory to virtual address space of process */
int (map_phys_mem_to_virtual)(uint16_t mode);
/* Enter in Graphics or Text mode */
int (vg_enter)(uint16_t mode);
/* Get the color of a specific rgb component */
int (get_rgb_component)(uint32_t color, uint8_t component_size, uint8_t component_position, uint8_t *component);

int (vg_draw_pixel)(uint8_t *buffer, uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hl)(uint8_t *buffer, uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (vg_draw_circle)(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
int (vg_draw_circle_to_buffer)(uint8_t *buffer, uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
int (vg_draw_line)(uint8_t *buffer, position_t pos1, position_t pos2, uint16_t radius, uint32_t color);

int (vg_draw_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);
int (vg_erase_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);

int (vg_draw_char)(const uint8_t character, uint16_t x, uint16_t y);
int (vg_draw_text)(char *string, uint16_t x, uint16_t y);
int (vg_draw_guess)(guess_word_t *gwt, uint16_t x, uint16_t y);

int (vg_draw_cursor)(cursor_image_t image, position_t pos);

int (vg_draw_button)(button_t *button);
int (vg_draw_buttons)(buttons_array_t *buttons);

int (vg_buffer_flip)();
int (vg_clear_buffer)(uint8_t buffer);

unsigned (get_vram_size)();
int (vg_copy_canvas_buffer)(uint8_t * buffer);
int (vg_clear_buffers)();

uint16_t (get_v_res)();
uint16_t (get_h_res)();
uint8_t (get_bytes_per_pixel)();
