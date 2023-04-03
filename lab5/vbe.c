#include "vbe.h"

extern uint8_t bytes_per_pixel;
extern unsigned int vram_base;  /* VRAM's physical addresss */
extern unsigned int vram_size;  /* VRAM's size, but you can use the frame buffer size instead */


extern uint8_t *video_mem;		/* Process (virtual) address to which VRAM is mapped */

extern unsigned h_res;	        /* Horizontal resolution in pixels */
extern unsigned v_res;	        /* Vertical resolution in pixels */


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
  printf("intno: %02x\n", reg86p.intno);
  printf("ah: %02x\n", reg86p.ah);
  printf("al: %02x\n", reg86p.al);
  printf("bx: %04x\n", reg86p.bx);
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
  /*
  for (int i = 0; i < bytes_per_pixel; i++){
    if (memcpy(&video_mem[index], &color, bytes_per_pixel) != 0) {
      printf("memcpy inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    printf("i = %s\n", i);
  }
  */
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
