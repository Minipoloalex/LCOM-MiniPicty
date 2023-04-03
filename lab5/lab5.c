// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "vbe.h"
#include "keyboard.h"

// bytes per pixel
uint8_t bytes_per_pixel;
unsigned int vram_base;  /* VRAM's physical addresss */
unsigned int vram_size;  /* VRAM's size, but you can use the frame buffer size instead */


uint8_t *video_mem;		/* Process (virtual) address to which VRAM is mapped */

unsigned int h_res;	        /* Horizontal resolution in pixels */
unsigned int v_res;	        /* Vertical resolution in pixels */

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (vg_enter(mode) != OK) return EXIT_FAILURE;
  sleep(delay);
  return vg_exit();
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  
  /*
  1- Subscribe keyboard
  2- Entrar mode
  3- Desenhar retangulo
  4- Ciclo do professor
  5- Unsubscribe
  6- Vg.exit
  */
  uint8_t bit_no;
  if(keyboard_subscribe_interrupts(&bit_no) != 0) return EXIT_FAILURE;
  
  if (map_phys_mem_to_virtual(mode) != OK){
    printf("map_phys_mem_to_virtual inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (vg_enter(mode) != OK) return EXIT_FAILURE;

  // Draw rectangle
  if (vg_draw_rectangle(x, y, width, height, color) != OK) return EXIT_FAILURE;

  int ipc_status, r;
  message msg;
  
  extern uint8_t scancode;
  extern int return_value;
  do {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with %d", r);
      }
      if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & BIT(bit_no)) {
              keyboard_ih();
              if (return_value) continue;
            }
            break;
          default:
            break;
        }
      }
    } while (scancode != BREAK_ESC);

  if(vg_exit() != 0) return EXIT_FAILURE;
  
  return keyboard_unsubscribe_interrupts();
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
