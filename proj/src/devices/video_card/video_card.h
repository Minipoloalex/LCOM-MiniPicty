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

// guess word
#define GUESS_POS_X 30
#define GUESS_POS_Y 800

// guess box
#define GUESS_BOX_X 15
#define GUESS_BOX_Y 780
#define GUESS_BOX_WIDTH 1024-30 //hres-30
#define GUESS_BOX_HEIGHT 40+FONT_HEIGHT

// round timer
#define ROUND_TIMER_X 900
#define ROUND_TIMER_Y 800

// colors
#define TRANSPARENT 0x000001
#define BLACK 0X000000
#define WHITE 0XFFFFFF

/**
 * @brief 
 * 
 */
bool (buffers_need_update)();
/**
 * @brief 
 * 
 */
void (set_needs_update)(bool value);
/**
 * @brief 
 * 
 */
int (setup_video_mode)(uint16_t mode);
/**
 * @brief Map physical memory to virtual address space of process
 * Gets the memory for all 3 buffers
 */
int (map_phys_mem_to_virtual)(uint16_t mode);
/**
 * @brief Enter in Graphics mode
 * 
 */
int (vg_enter)(uint16_t mode);

/**
 * @brief 
 * 
 */
int (vg_draw_pixel)(uint8_t *buffer, uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief 
 * 
 */
int (vg_draw_hl)(uint8_t *buffer, uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief 
 * 
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief 
 * 
 */
int (vg_draw_rectangle_to_buffer)(uint8_t *buffer, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief 
 * 
 */
int (vg_draw_circle)(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
/**
 * @brief 
 * 
 */
int (vg_draw_circle_to_buffer)(uint8_t *buffer, uint16_t x, uint16_t y, uint16_t radius, uint32_t color);
/**
 * @brief 
 * 
 */
int (vg_draw_line)(uint8_t *buffer, position_t pos1, position_t pos2, uint16_t radius, uint32_t color);

int (vg_draw_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);
int (vg_erase_xpm)(xpm_image_t *img, uint16_t x, uint16_t y);

int (vg_draw_char)(const uint8_t character, uint16_t x, uint16_t y);
int (vg_draw_text)(char *string, uint16_t x, uint16_t y);
int (vg_draw_guess)(guess_word_t *gwt, uint16_t x, uint16_t y);

int (vg_draw_cursor)(cursor_image_t image, position_t pos);

int (vg_draw_button)(button_t *button);
int (vg_draw_buttons)(buttons_array_t *buttons);
/**
 * @brief 
 * 
 */
int (vg_buffer_flip)();
/**
 * @brief 
 * 
 */
int (vg_clear_buffer)(uint8_t buffer);

/**
 * @brief 
 * 
 */
int (vg_draw_buffer)(uint8_t * buffer);
/**
 * @brief 
 * 
 */
int (vg_clear_buffers)();

/**
 * @brief 
 * 
 */
unsigned (get_vram_size)();
/**
 * @brief 
 * 
 */
uint16_t (get_v_res)();
/**
 * @brief 
 * 
 */
uint16_t (get_h_res)();
/**
 * @brief 
 * 
 */
uint8_t (get_bytes_per_pixel)();
