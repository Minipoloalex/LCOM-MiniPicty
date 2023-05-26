#include "video_card.h"

uint8_t bytes_per_pixel;
uint8_t bits_per_pixel;
unsigned int vram_base;  /* VRAM's physical addresss */
unsigned int vram_size;  /* VRAM's size, but you can use the frame buffer size instead */

#define BUFFER_NUMBER 3
static uint8_t* video_mem[BUFFER_NUMBER]; /* Process (virtual) address to which VRAM is mapped */
static uint8_t buffer_index = 0;
static bool needs_update = false;

bool (buffers_need_update)() {
  return needs_update;
}
void (set_needs_update)(bool value) {
  needs_update = value;
}

unsigned h_res;	        /* Horizontal resolution in pixels */
unsigned v_res;	        /* Vertical resolution in pixels */
vbe_mode_info_t vmi;

int (setup_video_mode)(uint16_t mode){
  if (map_phys_mem_to_virtual(mode) != OK){
    printf("map_phys_mem_to_virtual inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (vg_enter(mode) != OK) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

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

  if (reg86p.al != INVOKE_VBE_FUNCT || reg86p.ah != VBE_SUCCESS) {
    printf("r86.al: %02x ,r86.ah: %02x inside %s\n", reg86p.al, reg86p.ah, __func__);
    return EXIT_FAILURE;
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
  mr.mr_limit = mr.mr_base + 3 * vram_size;  
  
  int r;
  if((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)) != OK) {
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    return EXIT_FAILURE;
  }

  /* Map memory into buffers */
  for(int i = 0; i < BUFFER_NUMBER; i++){
    video_mem[i] = vm_map_phys(SELF, (void *)(mr.mr_base + i * vram_size), vram_size);
    if(video_mem[i] == MAP_FAILED) {
      printf("couldn't map video memory inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (vg_clear_buffer(i)) {
      printf("vg_clear_buffer inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  buffer_index = 1;
  set_needs_update(true);
  return EXIT_SUCCESS;
}

int (vg_buffer_flip)() {
  reg86_t reg86p;
  memset(&reg86p, 0, sizeof(reg86p));
  reg86p.intno = BIOS_VIDEO_SERVICES;
  reg86p.ah = INVOKE_VBE_FUNCT;
  reg86p.al = VERTICAL_RETRACE;
  reg86p.bx = SET_START_OF_DISPLAY;
  reg86p.cx = 0;
  reg86p.dx = buffer_index * v_res;
  if (sys_int86(&reg86p) != OK) {
    printf("sys_int86 inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (reg86p.al != INVOKE_VBE_FUNCT || reg86p.ah != VBE_SUCCESS) {
    printf("r86.al: %02x ,r86.ah: %02x inside %s\n", reg86p.al, reg86p.ah, __func__);
    return EXIT_FAILURE;
  }
  buffer_index = (buffer_index + 1) % BUFFER_NUMBER;

  set_needs_update(false);
  return EXIT_SUCCESS;
}

int (vg_clear_buffers)() {
  for (int i = 0; i < BUFFER_NUMBER; i++) {
    if (vg_clear_buffer(i)) {
      printf("vg_clear_buffer %d inside %s\n", i, __func__);
    }
  }
  return EXIT_SUCCESS;
}
int (vg_clear_buffer)(uint8_t buffer){
  if (memset(video_mem[buffer], 0, vram_size) == NULL) {
    printf("memset inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (vg_draw_pixel)(uint8_t *buffer, uint16_t x, uint16_t y, uint32_t color){
  if (x < 0 || x >= h_res || y < 0 || y >= v_res) {
    return EXIT_SUCCESS;
  }
  unsigned int index = (y * h_res + x) * bytes_per_pixel;
  if (color == xpm_transparency_color(XPM_8_8_8_8)) return EXIT_SUCCESS;
  memcpy(&buffer[index], &color, bytes_per_pixel);
  return EXIT_SUCCESS;
}

int (vg_draw_hl)(uint8_t *buffer, uint16_t x, uint16_t y, uint16_t len, uint32_t color){
  for(unsigned int i = x; i < x + len; i++){
    vg_draw_pixel(buffer, i, y, color);
  }
  return EXIT_SUCCESS;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (unsigned int verticalIndex = y; verticalIndex < y + height; verticalIndex++) {
    if (vg_draw_hl(video_mem[buffer_index], x, verticalIndex, width, color) != OK) {
      printf("vg_draw_hl inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int (vg_draw_circle)(uint16_t xc, uint16_t yc, uint16_t radius, uint32_t color){
  return vg_draw_circle_to_buffer(video_mem[buffer_index], xc, yc, radius, color);
}

/* Drawing a circle with Bresenham's algorithm */
int (vg_draw_circle_to_buffer)(uint8_t *buffer, uint16_t xc, uint16_t yc, uint16_t radius, uint32_t color){
  int x = 0;
  int y = radius;
  int d = 3 - 2 * radius;

  while (x <= y) {
    for (int i = xc - x; i <= xc + x; i++) {
        vg_draw_pixel(buffer, i, yc + y, color);
        vg_draw_pixel(buffer, i, yc - y, color);
    }
    for (int i = xc - y; i <= xc + y; i++) {
        vg_draw_pixel(buffer, i, yc + x, color);
        vg_draw_pixel(buffer, i, yc - x, color);
    }

    if (d < 0) {
        d = d + 4 * x + 6;
    } else {
        d = d + 4 * (x - y) + 10;
        y--;
    }
    x++;
  }
  return EXIT_SUCCESS;
}

/* Bresenham's line algorithm */
//TODO: Explore Xiaolin Wu's algorithm for drawing lines (anti-aliasing)
int (vg_draw_line)(uint8_t *buffer, position_t pos1, position_t pos2, uint16_t thickness, uint32_t color) {
    int x0 = pos1.x;
    int y0 = pos1.y;
    int x1 = pos2.x;
    int y1 = pos2.y;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;
    int e2;

    // draw first point of the line with specified color
    vg_draw_circle_to_buffer(buffer, x0, y0, thickness / 2, color);

    while (x0 != x1 || y0 != y1) {
        e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }

        vg_draw_circle_to_buffer(buffer, x0, y0, thickness / 2, color);
    }

    vg_draw_circle_to_buffer(buffer, x1, y1, thickness / 2, color);

    return EXIT_SUCCESS;
}

int (vg_copy_canvas_buffer)(uint8_t *buffer){
  if (memcpy(video_mem[buffer_index], buffer, vram_size) == NULL) {
    printf("memcpy inside %s\n", __func__);
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

  for (int row = y; row < y + img->height; row++) {
    for (int col = x; col < x + img->width; col++, colors += bytes_per_pixel) {
        uint32_t final_color = 0x00000000;
        memcpy(&final_color, colors, bytes_per_pixel);
        if (vg_draw_pixel(video_mem[buffer_index], col, row, final_color)) {
          printf("vg_draw_pixel inside %s\n", __func__);
          return EXIT_FAILURE;
        }
    }
  }
  return EXIT_SUCCESS;
}

int (vg_erase_xpm)(xpm_image_t *img, uint16_t x, uint16_t y) {
  for (int row = y; row < y + img->height; row++) {
    for (int col = x; col < x + img->width; col++) {
      if (vg_draw_pixel(video_mem[buffer_index], col, row, 0)) {
        printf("vg_draw_pixel inside %s\n", __func__);
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}

int (vg_draw_char)(const uint8_t character, uint16_t x, uint16_t y){
  uint8_t index;
  //printf("%c", character);
  if (character >= 'a' && character <= 'z') index = character - 'a';
  else if (character >= 'A' && character <= 'Z') index = character - 'A';
  else if (character >= '0' && character <= '9') index = character - '0' + 26;
  else {
    printf("character: '%c' not supported inside %s\n", character, __func__);
    return EXIT_SUCCESS;
  }

  xpm_map_t char_xpm = uppercase_alphabet[index];

  xpm_image_t loaded_char;
  uint8_t *colors = xpm_load(char_xpm, XPM_8_8_8_8, &loaded_char);
  
  if (colors == NULL || loaded_char.type == INVALID_XPM){
    colors == NULL ? printf("Cores nulas") : printf("XPM inválido");
    return EXIT_FAILURE;
  }

  loaded_char.bytes = colors;
  
  if (vg_draw_xpm(&loaded_char, x, y)) {
    printf("vg_draw_xpm inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}

int (vg_draw_text)(char *string, uint16_t x, uint16_t y){
  for (uint16_t xi = x; *string != 0; string++, xi += FONT_WIDTH){
    if (vg_draw_char(*string, xi, y)) {
      printf("vg_draw_char inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

int (vg_draw_guess)(guess_word_t *guess, uint16_t x, uint16_t y){
  
  /*printf("string: %s ", guess->string);
  for (size_t i = 0; i < guess->pointer; i++){
    printf("%c", guess->string[i]);
  }
  printf(" - pointer: %d \n", guess->pointer);*/
  //printf("drawing: ");
  for (uint16_t xi = x, i=0; i<guess->pointer; i++, xi+=FONT_WIDTH){
    if (vg_draw_char(guess->string[i], xi, y) != OK) return EXIT_FAILURE;
  }
  //printf("\n");
  return EXIT_SUCCESS;
}

int (vg_draw_cursor)(cursor_image_t image, position_t pos){
  xpm_map_t cursor = cursors[image];
  //if (get_cursor_xpm(image, &cursor)) return EXIT_FAILURE;
  
  xpm_image_t cursor_image;
  uint8_t *colors = xpm_load(cursor, XPM_8_8_8_8, &cursor_image);
  if (colors == NULL || cursor_image.type == INVALID_XPM) return EXIT_FAILURE;
  cursor_image.bytes = colors;

  if (vg_draw_xpm(&cursor_image, pos.x, pos.y)) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (vg_draw_button)(button_t *button) {
  if (vg_draw_rectangle(button->x, button->y, button->width, button->height, button->background_color)) {
    printf("vg_draw_rectangle inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (button->icon != NOTHING){
    xpm_map_t icon = icons[button->icon];
    xpm_image_t icon_image;
    uint8_t *colors = xpm_load(icon, XPM_8_8_8_8, &icon_image);
    if (colors == NULL || icon_image.type == INVALID_XPM) return EXIT_FAILURE;
    icon_image.bytes = colors;

    if (vg_draw_xpm(&icon_image, button->x + (button->width - icon_image.width)/2, button->y + (button->height - icon_image.height)/2)) {
      printf("vg_draw_xpm inside %s\n", __func__);
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }

  if (button->text != NULL) {
    vg_draw_text(button->text, 
              button->x+(button->width/2)-(strlen(button->text)*FONT_WIDTH)/2, 
              button->y+(button->height/2)-(FONT_HEIGHT)/2);
  }
  return EXIT_SUCCESS;
}

int (vg_draw_buttons)(buttons_array_t *buttons) {
  for (int i = 0; i < buttons->num_buttons; i++) {
    button_t button = buttons->buttons[i];
    if (vg_draw_button(&button)) {
      printf("vg_draw_button inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}

unsigned (get_vram_size)(){
  return vram_size;
}
