#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

#include "modules/interrupts/interrupts.h"
#include "modules/menu/menu.h"
#include "modules/game/game.h"

//TODO: remove this hardcoded definition
#define BIT(n) (1 << (n))

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

typedef enum {
  MENU,
  GAME,
} state_t;

int(proj_main_loop)(int argc, char *argv[]) {

  // Load resources

  // Subscribe interrupts
  if(subscribe_interrupts()) return EXIT_FAILURE;

  if (map_phys_mem_to_virtual(GRAPHICS_MODE_0) != OK){
    printf("map_phys_mem_to_virtual inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (vg_enter(GRAPHICS_MODE_0) != OK) return EXIT_FAILURE;
  
  // Draw the current state
  // TODO: Explore the table-based solution later
  state_t app_state = MENU;
  switch(app_state){
    case MENU:
      draw_menu();
      break;
    case GAME:
      draw_game();
      break;
  }

  // Game Loop
  int ipc_status, r;
  message msg;
  
  extern uint8_t scancode;
  extern int return_value;
  
  extern uint8_t keyboard_bit_no;
  extern uint8_t mouse_bit_no;
  extern uint8_t timer_bit_no;

  extern uint8_t packet_byte;
  extern uint8_t return_value_mouse;

  do {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with %d", r);
      }
      if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & BIT(keyboard_bit_no)) {
              keyboard_ih();
              if (return_value) continue;
            }
            if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
              mouse_ih();
            }
            if (msg.m_notify.interrupts & BIT(timer_bit_no)){
              timer_int_handler();
            }
            break;
          default:
            break;
        }
      }
    } while (scancode != BREAK_ESC);

  // Unload resources

  // Exit graphics mode
  if (vg_exit() != OK) return EXIT_FAILURE;

  // Unsubscribe interrupts
  if(unsubscribe_interrupts()) return 1;

  return 0;
}
/*
int (proj_main_loop)(int argc, char *argv[]) {
  if (argc != 1 || (strcmp(argv[0], "host") != 0 && strcmp(argv[0], "remote") != 0)) {
    printf("Usage: lcom_run proj <host|remote>\n");
    return EXIT_FAILURE;
  }
  unsigned short base_addr = SER_COM1;
  uint8_t bits_per_char = 8;
  uint8_t stop_bits = 1;
  int8_t parity = 1;
  uint32_t rate = SER_MAX_BITRATE;

  char *strings[] = {"Hello", "World", "Hello", "Again", "qwertyuiopasdfghjklzxcvbnm", "abcdefghij"};   // must not have dots (dot is the termination symbol)
  int stringc = 6;
  uint8_t is_transmitter = strcmp(argv[0], "host") ? 0 : 1; // 1 if host, 0 if remote
  printf("is_transmitter: %02x\n", is_transmitter);

  if (ser_test_fifo(base_addr, is_transmitter, bits_per_char, stop_bits, parity, rate, stringc, strings) != OK) {
    printf("Error in ser_test_fifo()\n");
    return EXIT_FAILURE;
  }

  printf("proj_main_loop() ended successfully\n");
  return EXIT_SUCCESS;
}
*/
