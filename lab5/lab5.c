// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "vbe.h"
#include "keyboard.h"
#include "timer.c"

uint8_t bytes_per_pixel;
uint8_t bits_per_pixel;
unsigned int vram_base;  /* VRAM's physical addresss */
unsigned int vram_size;  /* VRAM's size, but you can use the frame buffer size instead */


uint8_t *video_mem;		/* Process (virtual) address to which VRAM is mapped */

unsigned int h_res;	        /* Horizontal resolution in pixels */
unsigned int v_res;	        /* Vertical resolution in pixels */
vbe_mode_info_t vmi;

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
  uint8_t bit_no;
  if(keyboard_subscribe_interrupts(&bit_no) != 0) return EXIT_FAILURE;
  
  if (map_phys_mem_to_virtual(mode) != OK){
    printf("map_phys_mem_to_virtual inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (vg_enter(mode) != OK) return EXIT_FAILURE;
  
  unsigned int rec_width = h_res / no_rectangles;
  unsigned int rec_height = v_res / no_rectangles;
  uint32_t color;
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;

  for (int row = 0; row < no_rectangles; row++) {
    for (int col = 0; col < no_rectangles; col++) {
      if (mode == 0x105) {
        color = (first + (row * no_rectangles + col) * step) % (1 << bits_per_pixel);
        printf("col: %d, row: %d, color_index: %d\n", col, row, color);
      }
      else { /* mode is direct */
        // print all of the mask sizes
        printf("r size 0x%02x, g size 0x%02x b size 0x%02x ", vmi.RedMaskSize, vmi.GreenMaskSize, vmi.BlueMaskSize);
        printf("r pos 0x%02x, g pos 0x%02x b pos 0x%02x ", vmi.RedFieldPosition, vmi.GreenFieldPosition, vmi.BlueFieldPosition);
        uint8_t red_first = 0;
        uint8_t green_first = 0;
        uint8_t blue_first = 0;
        get_rgb_component(first, vmi.RedMaskSize, vmi.RedFieldPosition, &red_first);
        get_rgb_component(first, vmi.GreenMaskSize, vmi.GreenFieldPosition, &green_first);
        get_rgb_component(first, vmi.BlueMaskSize, vmi.BlueFieldPosition, &blue_first);
        red = (red_first + col * step) % (1 << vmi.RedMaskSize);
	      green = (green_first + row * step) % (1 << vmi.GreenMaskSize);
	      blue = (blue_first + (col + row) * step) % (1 << vmi.BlueMaskSize);
        color = (red << vmi.RedFieldPosition) | (green << vmi.GreenFieldPosition) | (blue << vmi.BlueFieldPosition);
        printf("col: %d, row: %d, color_rgb: 0x%08x, %ld\n", col, row, color, color);
      }
      if (vg_draw_rectangle(col * rec_width, row * rec_height, rec_width, rec_height, color) != OK) {
          printf("vg_draw_rectangle inside %s\n", __func__);
          return EXIT_FAILURE;
        }
    }
  }
  printf("Finished drawing rectangles with success\n");

  int r, ipc_status;
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

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  uint8_t bit_no;
  if(keyboard_subscribe_interrupts(&bit_no) != 0) return EXIT_FAILURE;
  
  if (map_phys_mem_to_virtual(0x105) != OK){
    printf("map_phys_mem_to_virtual inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (vg_enter(0x105) != OK) return EXIT_FAILURE;


  
  xpm_image_t xpm_image;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &xpm_image);
  if (colors == NULL) {
    printf("xpm_load inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  xpm_image.bytes = colors;
  if (xpm_image.type == INVALID_XPM) {
    printf("xpm_image.type inside %s\n", __func__);
    return EXIT_FAILURE;
  }


  if (vg_draw_xpm(&xpm_image, x, y) != OK) {
    printf("vg_draw_xpm inside %s\n", __func__);
    return EXIT_FAILURE;
  }


  int r, ipc_status;
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

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* movement only along one axis */  /* assume speed is positive */
  uint8_t keyboard_bit_no;
  uint8_t timer_bit_no;
  if(keyboard_subscribe_interrupts(&keyboard_bit_no) != 0) return EXIT_FAILURE;
  if(timer_subscribe_int(&timer_bit_no) != 0) return EXIT_FAILURE;

  if (map_phys_mem_to_virtual(0x105) != OK){
    printf("map_phys_mem_to_virtual inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (vg_enter(0x105) != OK) return EXIT_FAILURE;

  xpm_image_t xpm_image;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &xpm_image);
  xpm_image.bytes = colors;
  if (colors == NULL) {
    printf("xpm_load inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (xpm_image.type == INVALID_XPM) {
    printf("xpm_image.type inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if (vg_draw_xpm(&xpm_image, xi, yi) != OK) {
    printf("vg_draw_xpm inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  int r, ipc_status;
  message msg;
  extern uint8_t scancode;
  extern int return_value;
  extern int counter;
  int ints_per_frame = sys_hz() / fr_rate;
  do {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with %d", r);
      }
      if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
              timer_int_handler();
              if (counter % ints_per_frame == 0) {  // general frame rate
                /* check with professor if processing the interrupt here is ok */
                if (speed > 0 || (speed < 0 && counter % (ints_per_frame * abs(speed)) == 0)) {
                  if (vg_erase_xpm(&xpm_image, xi, yi) != OK) {
                  printf("vg_erase_xpm inside %s\n", __func__);
                  return EXIT_FAILURE;
                  }
                  if (update_position(&xi, xf, &yi, yf, speed) != OK) {
                    printf("update_position inside %s\n", __func__);
                    return EXIT_FAILURE;
                  }
                  if (vg_draw_xpm(&xpm_image, xi, yi) != OK) {
                    printf("vg_draw_xpm inside %s\n", __func__);
                    return EXIT_FAILURE;
                  }
                }
              }
            }
            if (msg.m_notify.interrupts & BIT(keyboard_bit_no)) {
              keyboard_ih();
              if (return_value) continue;
            }
            break;
          default:
            break;
        }
      }
    } while (scancode != BREAK_ESC);

  if(vg_exit() != OK) return EXIT_FAILURE;

  if(timer_unsubscribe_int() != OK) return EXIT_FAILURE;
  return keyboard_unsubscribe_interrupts();
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
