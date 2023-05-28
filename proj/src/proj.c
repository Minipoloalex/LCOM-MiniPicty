#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "modules/interrupts/interrupts.h"
#include "modules/menu/menu.h"
#include "modules/resources/resources.h"
#include "modules/menu/player_menu/player_menu.h"
#include "modules/game/game.h"
#include "model/state/state.h"

/** @defgroup proj proj
 * @{
 *
 * @brief File containing the main loop of the game
 */

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

// App State variable
state_t *app_state = NULL;

int(proj_main_loop)(int argc, char *argv[]) {
  if (argc != 1 || (strcmp(argv[0], "host") != 0 && strcmp(argv[0], "remote") != 0)) {
    printf("Usage: lcom_run proj <host|remote>\n");
    return EXIT_FAILURE;
  }
  bool isTransmitter = strcmp(argv[0], "host") == 0;
  if (ser_init()) {
    printf("ser_init inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (rtc_init() != OK) {
    printf("rtc_init inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  
  // Subscribe interrupts
  if(subscribe_interrupts()) return EXIT_FAILURE;

  // Enter video mode
  if(setup_video_mode(GRAPHICS_MODE_4)){
    printf("setup_video_mode inside %s\n", __func__);
    return EXIT_FAILURE;
  }  

  // Load resources
  Resources* resources = load_resources();
  if(resources == NULL){
    printf("load_resources inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  app_state = create_state();
  if (app_state == NULL) {
    printf("create_state inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  // Setup the app states
  if (setup_menu(app_state, resources) != OK) {
    printf("setup inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (setup_game(isTransmitter, app_state, resources) != OK) {
    printf("setup inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  // Setup the initial state: Menu
  transition_to_menu(app_state);

  // Game Loop
  int ipc_status, r;
  message msg;
  
  extern uint8_t scancode;
  extern int return_value;
  extern uint8_t keyboard_bit_no;
  
  extern uint8_t mouse_bit_no;
  extern uint8_t return_value_mouse;
  
  extern uint8_t timer_bit_no;

  extern uint8_t ser_bit_no;
  extern uint8_t ser_return_value;

  extern uint8_t rtc_bit_no;
  extern int rtc_return_value;
  do {
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with %d", r);
      }
      if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & BIT(keyboard_bit_no)) {
              keyboard_ih();
              app_state->process_keyboard(app_state);
            }
            if (msg.m_notify.interrupts & BIT(mouse_bit_no)) {
              mouse_ih();
              if (return_value_mouse == EXIT_SUCCESS){
                mouse_process_packet_byte();
                if (packet_is_ready()) {
                  app_state->process_mouse(app_state);
                }
              }
            }
            if (msg.m_notify.interrupts & BIT(timer_bit_no)) {
              timer_int_handler();
              app_state->process_timer(app_state);
              app_state->draw(app_state);
            }
            if (msg.m_notify.interrupts & BIT(ser_bit_no)){
              ser_ih_fifo();
              if (ser_return_value == EXIT_SUCCESS){
                app_state->process_serial(app_state);
              }
            }
            if (msg.m_notify.interrupts & BIT(rtc_bit_no)) {
              rtc_ih();
              if (rtc_return_value == EXIT_SUCCESS) {
                app_state->process_rtc(app_state);
              }
            }
            break;
          default:
            break;
        }
      }
    } while (app_state->running_app && scancode != BREAK_ESC);

  // Unload resources
  destroy_game();
  destroy_menu();
  unload_resources(resources);
  destroy_state(app_state);

  // Stop serial communication
  delete_ser();

  // Exit graphics mode
  if (vg_exit() != OK) return EXIT_FAILURE;
  // Unsubscribe interrupts
  if(unsubscribe_interrupts()) return 1;

  return 0;
}
