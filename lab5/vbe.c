#include "vbe.h"

extern uint8_t bpp;
extern unsigned int vram_base;  /* VRAM's physical addresss */
extern unsigned int vram_size;  /* VRAM's size, but you can use the frame buffer size instead */


extern char *video_mem;		/* Process (virtual) address to which VRAM is mapped */

extern unsigned h_res;	        /* Horizontal resolution in pixels */
extern unsigned v_res;	        /* Vertical resolution in pixels */
extern unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


int (vg_enter)(uint16_t mode) {
  vbe_mode_info_t vmi_p;
  if (vbe_get_mode_info(mode, &vmi_p) != 0) return EXIT_FAILURE;
  
  struct reg86 reg86p;

  memset(&reg86p, 0, sizeof(reg86p));
  reg86p.intno = 0x10; /* BIOS video services */
  reg86p.ah = 0x4F;    /* Set Video Mode function */
  reg86p.al = 0x02;    /* 80x25 graphics mode */
  reg86p.bx = mode | BIT(14);
  /* Make the BIOS call */

  if( sys_int86(&reg86p) != OK ) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (vg_draw_pixel)(in_port_t x, in_port_t y, gid_t color){
  void * pos = video_mem + ((y * h_res + x) * bpp);
  
  for (int i = 0; i < bpp; i++){
    if (memcpy(pos, &color, bpp) != 0) {
      printf("adoro lcom");
      return EXIT_FAILURE; 
    }
  }
  return EXIT_SUCCESS;
}

int (vg_draw_hline)(in_port_t x, in_port_t y, in_port_t len, gid_t color){
  unsigned limit_x = x + len;
  for(unsigned int i = x; i < limit_x; i++){
    if (vg_draw_pixel(i, y, color) != OK) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (vg_draw_rectangle)(in_port_t x, in_port_t y, in_port_t width, in_port_t height, gid_t color){
  unsigned limit_y = y + height;
  for (unsigned int verticalIndex = y; verticalIndex < limit_y; verticalIndex++) {
    if (vg_draw_hline(x, verticalIndex, width, color) != OK) return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
