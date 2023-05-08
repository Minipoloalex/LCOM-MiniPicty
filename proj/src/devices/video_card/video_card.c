#include "video_card.h"

uint8_t bytes_per_pixel;
uint8_t bits_per_pixel;
unsigned int vram_base;  /* VRAM's physical addresss */
unsigned int vram_size;  /* VRAM's size, but you can use the frame buffer size instead */

uint8_t *video_mem;		/* Process (virtual) address to which VRAM is mapped */

unsigned h_res;	        /* Horizontal resolution in pixels */
unsigned v_res;	        /* Vertical resolution in pixels */
vbe_mode_info_t vmi;

int (vg_enter)(uint16_t mode) {
  
  memset(&vmi, 0, sizeof(vmi));
  if (vbe_get_mode_info(mode, &vmi) != 0) return EXIT_FAILURE;

  struct reg86 reg86p;
  memset(&reg86p, 0, sizeof(reg86p));

  reg86p.intno = BIOS_VIDEO_SERVICES;
  reg86p.ah = INVOKE_VBE_FUNCT;    
  reg86p.al = SET_VBE_MODE;   
  reg86p.bx = mode | LINEAR_FRAMEBUFFER;
  
  /* Make the BIOS call */
  if(sys_int86(&reg86p) != OK) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
  if (x >= h_res || y >= v_res) {
    printf("x or y outside limits inside %s\n", __func__);
    return EXIT_SUCCESS;
  }
  unsigned int index = (y * h_res + x) * bytes_per_pixel;
  memcpy(&video_mem[index], &color, bytes_per_pixel);
  return EXIT_SUCCESS;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for(unsigned int i = x; i < x + len; i++){
    if (vg_draw_pixel(i, y, color) != OK) {
      printf("vg_draw_pixel inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (unsigned int verticalIndex = y; verticalIndex < y + height; verticalIndex++) {
    if (vg_draw_hline(x, verticalIndex, width, color) != OK) {
      printf("vg_draw_hline inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int (map_phys_mem_to_virtual)(uint16_t mode) {
  vbe_mode_info_t vmi_p;
  memset(&vmi_p, 0, sizeof(vmi_p));
  if (vbe_get_mode_info(mode, &vmi_p) != 0) return EXIT_FAILURE;

  bytes_per_pixel = (vmi_p.BitsPerPixel + 7) / 8; // Rounding by excess
  bits_per_pixel = vmi_p.BitsPerPixel;

  h_res = vmi_p.XResolution;
  v_res = vmi_p.YResolution;

  vram_size = h_res * v_res * bytes_per_pixel;
  vram_base = vmi_p.PhysBasePtr;
  
  struct minix_mem_range mr;
  mr.mr_base = (phys_bytes) vram_base;	
  mr.mr_limit = mr.mr_base + vram_size;  
  
  int r;
  if((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return EXIT_FAILURE;
  }

  /* Map memory */
  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
  if(video_mem == MAP_FAILED) {
    printf("couldn't map video memory inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (get_rgb_component)(uint32_t color, uint8_t component_size, uint8_t component_position, uint8_t *component) {
  uint8_t mask = BIT(component_size) - 1;
  uint8_t final = (uint8_t) (color >> component_position & (mask));
  *component = final;

  return EXIT_SUCCESS;
}

int (vg_draw_xpm)(xpm_image_t *img, uint16_t x, uint16_t y) {
  uint8_t *colors = img->bytes;

  for (int row = y; row < y + img->width; row++) {
    for (int col = x; col < x + img->height; col++) {
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
  for (int row = y; row < y + img->height; row++) {
    for (int col = x; col < x + img->width; col++) {
      if (vg_draw_pixel(col, row, 0)) {
        printf("vg_draw_pixel inside %s\n", __func__);
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}
