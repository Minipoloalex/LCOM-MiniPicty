#include "video_card.h"


extern uint8_t bytes_per_pixel;
extern uint8_t bits_per_pixel;
extern unsigned int vram_base;  /* VRAM's physical addresss */
extern unsigned int vram_size;  /* VRAM's size, but you can use the frame buffer size instead */


extern uint8_t *video_mem;		/* Process (virtual) address to which VRAM is mapped */

extern unsigned h_res;	        /* Horizontal resolution in pixels */
extern unsigned v_res;	        /* Vertical resolution in pixels */
extern vbe_mode_info_t vmi;


int (vg_enter)(uint16_t mode) {
  vbe_mode_info_t vmi_p;
  memset(&vmi_p, 0, sizeof(vmi_p));
  if (vbe_get_mode_info(mode, &vmi_p) != 0) return EXIT_FAILURE;
  vmi = vmi_p;  // copy
  struct reg86 reg86p;

  memset(&reg86p, 0, sizeof(reg86p));
  reg86p.intno = 0x10; /* BIOS video services */
  reg86p.ah = 0x4F;    /* Set Video Mode function */
  reg86p.al = 0x02;    /* 80x25 graphics mode */
  reg86p.bx = mode | BIT(14); /* Set bit 14: linear framebuffer */
  reg86p.bx &= 0x7FFF;  /* bit 15 of the BX register cleared, thus ensuring that the display memory is cleared after switching to the desired graphics mode.*/
  
  /* Make the BIOS call */
  if( sys_int86(&reg86p) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  if (x >= h_res || y >= v_res) {
    // printf("x or y outside limits inside %s\n", __func__);
    return EXIT_SUCCESS;
  }
  unsigned int index = (y * h_res + x) * bytes_per_pixel;
  // printf("index: %lu\n", index);
  memcpy(&video_mem[index], &color, bytes_per_pixel);
  return EXIT_SUCCESS;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  unsigned limit_x = x + len;
  for(unsigned int i = x; i < limit_x; i++){
    if (vg_draw_pixel(i, y, color) != OK) {
      printf("vg_draw_pixel inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  unsigned limit_y = y + height;
  // this print is already wrong
  for (unsigned int verticalIndex = y; verticalIndex < limit_y; verticalIndex++) {
    if (vg_draw_hline(x, verticalIndex, width, color) != OK) {
      printf("vg_draw_hline inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int (map_phys_mem_to_virtual)(uint16_t mode) {
  // Map phys memory to virtual address space of process
  
  vbe_mode_info_t vmi_p;
  memset(&vmi_p, 0, sizeof(vmi_p));
  if (vbe_get_mode_info(mode, &vmi_p) != 0) return EXIT_FAILURE;

  bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8; // rounding by excess
  bits_per_pixel = vmi_p.BitsPerPixel;

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;

  vram_size = h_res * v_res * bytes_per_pixel;
  vram_base = vmi_p.PhysBasePtr;
  
  struct minix_mem_range mr;

  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  
  
  int r;
  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return EXIT_FAILURE;
  }

  // printf("mr_base: %lu, mr_size: %lu\n", mr.mr_base, vram_size);
  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  // printf("mr_base: %lu, mr_size: %lu\n", mr.mr_base, vram_size);  
  if(video_mem == MAP_FAILED) {
    panic("couldn't map video memory");
    printf("couldn't map video memory inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (get_rgb_component)(uint32_t color, uint8_t component_size, uint8_t component_position, uint8_t *component) {
  uint32_t shifted = color >> component_position;
  uint8_t mask = BIT(component_size) - 1;
  uint8_t final = (uint8_t) (shifted & (mask));
  *component = final;

  return EXIT_SUCCESS;
}

int (vg_draw_xpm)(xpm_image_t *img, uint16_t x, uint16_t y) {
  uint8_t *colors = img->bytes;
  uint16_t width = img->width;
  uint16_t height = img->height;

  uint16_t limit_y = y + height;
  uint16_t limit_x = x + width;
  for (int row = y; row < limit_y; row++) {
    for (int col = x; col < limit_x; col++) {
      if (vg_draw_pixel(col, row, *colors)) {
        printf("vg_draw_pixel inside %s\n", __func__);
        return EXIT_FAILURE;
      }
      colors++;
    }
  }
  return EXIT_SUCCESS;
}

int (vg_erase_xpm)(xpm_image_t *img, uint16_t x, uint16_t y) {
  uint16_t width = img->width;
  uint16_t height = img->height;

  uint16_t limit_y = y + height;
  uint16_t limit_x = x + width;
  for (int row = y; row < limit_y; row++) {
    for (int col = x; col < limit_x; col++) {
      if (vg_draw_pixel(col, row, 0)) {
        printf("vg_draw_pixel inside %s\n", __func__);
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}

int (update_position)(uint16_t *x, uint16_t xf, uint16_t *y, uint16_t yf, int16_t speed) {
  uint16_t x_val = *x;
  uint16_t y_val = *y;
  if (x_val == xf && y_val == yf) return EXIT_SUCCESS;
  if (x_val != xf && y_val != yf) return EXIT_FAILURE;
  
  uint16_t diff = speed;
  if (speed < 0) {
    diff = 1;
  }

  /* This function could have problems because of overflow/underflow: if x - diff is < 0 */
  if (x_val < xf) {
    *x = x_val + diff;
    if (*x > xf) *x = xf;
  }
  else if (x_val > xf) {
    *x = x_val - diff;
    if (*x < xf) *x = xf;
  }
  else if (y_val < yf) {
    *y = y_val + diff;
    if (*y > yf) *y = yf;
  }
  else if (y_val > yf) {
    *y = y_val - diff;
    if (*y < yf) *y = yf;
  }
  return EXIT_SUCCESS;
}
