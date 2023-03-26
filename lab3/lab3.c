#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

uint32_t cnt_sys_inb = 0;

int(kbd_test_scan)() {
  uint8_t keyboard_bit_no;
  if (keyboard_subscribe_int(&keyboard_bit_no)) return 1;
  
  int r;
  message msg;
  int ipc_status;
  
  int index = 0;
  uint8_t scancodes_array[2];
  extern uint8_t scancode;
  extern int return_value;
  do {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with %d", r);
    }
    if (is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & BIT(keyboard_bit_no)) {
            kbc_ih();
            if (return_value) continue;

            scancodes_array[index] = scancode;
            if (index == 0 && scancode == SCANCODE_TWO_BYTES) {
              index = 1;
            }
            else {
              if (kbd_print_scancode(!(scancode & BREAK_CODE), index + 1, scancodes_array))
                return 1;
              index = 0;
            }
          }
          break;
        default:
          break;
      }
    }
  } while (scancode != ESC_BREAK_CODE);

  if (keyboard_unsubscribe_int()) return 1;

  return kbd_print_no_sysinb(cnt_sys_inb);
}

int(kbd_test_poll)() {
  int index = 0;
  uint8_t scancodes_array[2];
  uint8_t scancode;
  do {
    if (read_kbc_output(KBC_OUT_REG, &scancode)) continue;
    scancodes_array[index] = scancode;
    if (index == 0 && scancode == SCANCODE_TWO_BYTES) {
      index = 1;
    }
    else {
      if (kbd_print_scancode(!(scancode & BREAK_CODE), index + 1, scancodes_array))
        return 1;
      index = 0;
    }

  } while (scancode != ESC_BREAK_CODE);
  
  if (keyboard_enable_int()) return 1;

  return kbd_print_no_sysinb(cnt_sys_inb);
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
