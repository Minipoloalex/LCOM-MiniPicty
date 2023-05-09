#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "modules/interrupts/interrupts.h"
#include "modules/menu/menu.h"
#include "modules/game/player_drawer/player_drawer.h"
#include "modules/menu/player_menu/player_menu.h"
#include "modules/game/game.h"
#include "model/player/player.h"
#include "model/button/button.h"


int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");
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
  if (argc != 1 || (strcmp(argv[0], "host") != 0 && strcmp(argv[0], "remote") != 0)) {
    printf("Usage: lcom_run proj <host|remote>\n");
    return EXIT_FAILURE;
  }

  // Setuping Serial Port
  bool isTransmitter = strcmp(argv[0], "host") == 0;
  if (ser_init(0x3F8, 115200, 8, 1, 1)) {
    printf("ser_init inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  printf("isTransmitter: %d\n", isTransmitter);
  
  // Load resources

  // Subscribe interrupts
  if(subscribe_interrupts()) return EXIT_FAILURE;

  if (map_phys_mem_to_virtual(GRAPHICS_MODE_0) != OK){
    printf("map_phys_mem_to_virtual inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (vg_enter(GRAPHICS_MODE_0) != OK) return EXIT_FAILURE;
  
  // TODO: Explore the table-based solution later

  // MUST CHANGE THESE TO START ON DIFFERENT STATES (all that is needed is changing these)
  state_t app_state = GAME; // or MENU
  setup_game(); // or setup_menu();
  
  printf("Finished setup\n");
  // Game Loop
  int ipc_status, r;
  message msg;
  
  extern uint8_t scancode;
  extern int return_value;
  
  extern uint8_t keyboard_bit_no;
  extern uint8_t mouse_bit_no;
  extern uint8_t timer_bit_no;

  extern uint8_t ser_bit_no;
  extern uint8_t ser_return_value;

  extern uint8_t return_value_mouse;

  do {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with %d", r);
      }
      if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & BIT(keyboard_bit_no)) {
              printf("Received interrupt from keyboard\n");
              keyboard_ih();
            }
            if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
              printf("Received interrupt from mouse\n");
              mouse_ih();
              if (return_value_mouse == EXIT_SUCCESS){
                mouse_process_packet_byte();
                if (packet_is_ready()) {
                  switch (app_state) {
                    case GAME:
                      game_process_mouse();
                      break;
                    case MENU:
                      menu_process_mouse();
                      break;
                  }
                }
              }
            }
            if (msg.m_notify.interrupts & BIT(timer_bit_no)){
              printf("Received interrupt from timer\n");
              timer_int_handler();
              switch(app_state){
                case MENU:
                  draw_menu();
                  break;
                case GAME:
                  draw_game();
                  break;
              }
            }
            if (msg.m_notify.interrupts & BIT(ser_bit_no)){
              printf("Received interrupt from serial port\n");
              //ser_ih_fifo();
              ser_return_value = EXIT_FAILURE;
              if (ser_return_value == EXIT_SUCCESS){
                switch (app_state) {
                  case GAME:
                    game_process_serial();
                    break;
                  case MENU:
                    //menu_process_serial();
                    break;
                }
              }
            }
            break;
          default:
            break;
        }
      }
    } while (scancode != BREAK_ESC);
  // Unload resources
  switch (app_state) {
    case GAME:
      destroy_game();
      break;
    case MENU:
      destroy_menu();
      break;
  }

  // Stop serial communication
  delete_ser();

  // Exit graphics mode
  if (vg_exit() != OK) return EXIT_FAILURE;
  printf("ending\n");
  // Unsubscribe interrupts
  if(unsubscribe_interrupts()) return 1;

  return 0;
}

/*
 * TODOs:
 * call ser_init() somewhere, that calls
 * ser_read_bytes_from_receiver_queue
 * ser_write_bytes_to_transmitter_queue
 * ser_write_char at beggining to start communication and interrupts
 */







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
