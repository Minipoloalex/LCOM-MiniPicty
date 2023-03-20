// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "mouse.h"

extern int return_value_mouse;
extern int return_value_keyboard;

extern uint8_t scancode_byte;
extern uint8_t packet_byte;

// Any header files included below this line should have been created by you
int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {
  struct packet packet;
  uint8_t mouse_bit_no;
  
  if(mouse_subscribe_interrupts(&mouse_bit_no) != 0) return EXIT_FAILURE;
  if(mouse_enable_data_report() != 0) return EXIT_FAILURE;
  
  int ipc_status;
  message msg;
  int index = 0;
  
  while(cnt > 0){
    int r;
    if((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
  
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & BIT(mouse_bit_no)){
            mouse_ih();
            if (return_value_mouse != 0) {
              continue;
            }
            
            packet.bytes[index] = packet_byte;
            if(index == 0 && (packet_byte & FIRST_BYTE_VALIDATION)){
              packet.lb = LB(packet_byte);
              packet.rb = RB(packet_byte);
              packet.mb = MB(packet_byte);
              
              packet.x_ov = X_OV(packet_byte);
              packet.y_ov = Y_OV(packet_byte);

              packet.delta_x = DELTA_X(packet_byte) ? 0xFF00 : 0x0000;
              packet.delta_y = DELTA_Y(packet_byte) ? 0xFF00 : 0x0000;

              index++;
            }
            else if(index == 1){
              packet.delta_x |= packet_byte;
              index++;
            }
            else if(index == 2){
              packet.delta_y |= packet_byte;
              index = 0;
              mouse_print_packet(&packet);
              cnt--;
            }else{
              //choura
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

  if(mouse_disable_data_report() != 0) return EXIT_FAILURE;
  if(mouse_unsubscribe_interrupts() != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t timer0_bit_no;
  uint8_t mouse_bit_no;
  
  if(timer_subscribe_int(timer0_bit_no) != 0) return EXIT_FAILURE;
  if(mouse_subscribe_interrupts(&mouse_bit_no) != 0) return EXIT_FAILURE;
  if(mouse_enable_data_report() != 0) return EXIT_FAILURE;
  
  int ipc_status;
  message msg;

  uint8_t cnt = 0;
  uint8_t n = idle_time / (int) sys_hz();
  
  while(cnt < n){
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
            cnt++;
          }
          if (msg.m_notify.interrupts & BIT(mouse_bit_no)){
            mouse_ih();
            cnt = 0;
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

  if(timer_unsubscribe_int() != 0) return EXIT_FAILURE;
  if(mouse_disable_data_report() != 0) return EXIT_FAILURE;
  if(mouse_unsubscribe_interrupts() != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
