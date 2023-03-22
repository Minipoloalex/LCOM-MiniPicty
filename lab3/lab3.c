#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include "i8042.h"
#include "keyboard.h"
#include "i8254.h"

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

extern int return_value;
extern uint8_t scancode;
uint32_t cnt_sysinb = 0;

int(kbd_test_scan)() {
  uint8_t bit_no_keyboard;
  if(keyboard_subscribe_interrupts(&bit_no_keyboard) != 0) return EXIT_FAILURE;
  
  int ipc_status;
  message msg;
  uint8_t array[2];
  int i = 0;
  while(scancode != BREAK_ESC){

    int r;
    if((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(bit_no_keyboard)) { // BIT(KEYBOARD_IRQ)

            kbc_ih();
            
            if(return_value == 0){
              if(i == 0){
                array[i] = scancode;
                if (scancode == TWO_BYTES) {
                  i = 1;
                }
                else kbd_print_scancode(!(scancode & BREAK_CODE), i + 1, array);
              }
              else {
                array[i] = scancode;
                kbd_print_scancode(!(scancode & BREAK_CODE), i + 1, array);
                i = 0;
              }
            }
          }
          break;
        default:
          break; 
          /* no other notifications expected: do nothi */
      }
    } else { 
        /* 
          received a standard message, not a notification
          no standard messages expected: do nothing 
        */
    }
  }

  if (keyboard_unsubscribe_interrupts() != 0) return EXIT_FAILURE;
  if (kbd_print_no_sysinb(cnt_sysinb) != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}


int(kbd_test_poll)() {
  uint8_t array[2];
  bool reading2bytes = false;
  int ret = 0;
  while (scancode != BREAK_ESC){
    
    ret = read_KBC_output(KBC_OUT_REG, &scancode);
  
    if(ret != 0) continue;
    
    int i = reading2bytes ? 1 : 0;
    array[i] = scancode;
    
    if(reading2bytes){
      kbd_print_scancode(!(scancode & BREAK_CODE), 2, array);
      reading2bytes = false;
    }
    else if(scancode == TWO_BYTES){
      reading2bytes = true;
    }
    else{
      kbd_print_scancode(!(scancode & BREAK_CODE), 1, array);
    }

  }
  if(keyboard_restore() != 0) return EXIT_FAILURE;

  if (kbd_print_no_sysinb(cnt_sysinb) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int(kbd_test_timed_scan)(uint8_t max_seconds) {
  uint8_t max_counter = max_seconds * (int) sys_hz();

  uint8_t timer0_bit_no;
  uint8_t keyboard_bit_no;
  if (timer_subscribe_int(&timer0_bit_no) != 0) return EXIT_FAILURE;
  if (keyboard_subscribe_interrupts(&keyboard_bit_no) != 0) return EXIT_FAILURE;
  
  int ipc_status;
  message msg;
  
  uint8_t array[2];
  int i = 0;
  extern int counter;
  while(scancode != BREAK_ESC && counter < max_counter ){  

    int r;
    if((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(timer0_bit_no)){
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & BIT(keyboard_bit_no)) { // BIT(KEYBOARD_IRQ)
            
            counter = 0;

            kbc_ih();
            
            if(return_value == 0){
              if(i == 0){
                array[i] = scancode;
                if (scancode == TWO_BYTES) {
                  i = 1;
                }
                else kbd_print_scancode(!(scancode & BREAK_CODE), i + 1, array);
              }
              else {
                array[i] = scancode;
                kbd_print_scancode(!(scancode & BREAK_CODE), i + 1, array);
                i = 0;
              }
            }
          }
          break;
        default:
          break;
          /* no other notifications expected: do nothing */
      }
    } else {
        /* 
          received a standard message, not a notification
          no standard messages expected: do nothing
        */
    }
  }

  if (keyboard_unsubscribe_interrupts() != 0) return EXIT_FAILURE;
  if (timer_unsubscribe_int() != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
