// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>

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
  
  if(mouse_subscribe_interrupts(&mouse_bit_no) != 0) {
    printf("mouse_subscribe_interrupts inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (mouse_disable_int() != 0) {
    printf("mouse_disable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_enable_data_report() != 0) {
    printf("mouse_enable_data_report inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (mouse_enable_int() != 0) {
    printf("mouse_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  
  int ipc_status, r;
  message msg;
  uint8_t index = 0;
  
  while(cnt > 0){
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
            if (mouse_get_packet(&packet, &index, packet_byte) != 0) return EXIT_FAILURE;
            if (index == 3) {
              index = 0;
              mouse_print_packet(&packet);
              cnt--;
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
  if (mouse_disable_int()) {
    printf("mouse_disable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_disable_data_report() != 0) {
    printf("mouse_disable_data_report inside %s\n", __func__);
    return EXIT_FAILURE;    
  }

  if (mouse_enable_int() != 0) {
    printf("mouse_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_unsubscribe_interrupts() != 0) return EXIT_FAILURE;  
  return EXIT_SUCCESS;
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t timer0_bit_no;
  uint8_t mouse_bit_no;
  
  if(timer_subscribe_int(&timer0_bit_no) != 0) return EXIT_FAILURE;  
  if(mouse_subscribe_interrupts(&mouse_bit_no) != 0) return EXIT_FAILURE;

  if (mouse_disable_int() != 0) {
    printf("mouse_disable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_enable_data_report() != 0) {
    printf("mouse_enable_data_report inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (mouse_enable_int() != 0) {
    printf("mouse_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  
  int ipc_status;
  message msg;

  extern int counter;
  int max_counter = idle_time * (int) sys_hz();
  
  struct packet packet;
  uint8_t index = 0;

  while(counter < max_counter){
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
          if (msg.m_notify.interrupts & BIT(mouse_bit_no)){
            mouse_ih();
            counter = 0;
            if (return_value_mouse != 0) {
              continue;
            }
            if (mouse_get_packet(&packet, &index, packet_byte) != 0) return EXIT_FAILURE;
            if (index == 3) {
              index = 0;
              mouse_print_packet(&packet);
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

  if(timer_unsubscribe_int() != 0) return EXIT_FAILURE;
  
  if (mouse_disable_int() != 0) {
    printf("mouse_disable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_disable_data_report() != 0) {
    printf("mouse_disable_data_report inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (mouse_enable_int() != 0) {
    printf("mouse_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  if(mouse_unsubscribe_interrupts() != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}











// States
typedef enum {INIT, DRAW_LEFT, VERTEX, DRAW_RIGHT, FINAL} state_t;

static state_t state = INIT;

// struct mouse_ev *(mouse_detect_event)(struct packet *pp) {

// }

void process_event(struct packet* pp, uint8_t x_len, uint8_t tolerance) {

  static int x = 0;
  static int y = 0;

  bool only_lb_pressed, only_rb_pressed, none_pressed, correct_slope, min_dist, tolerant;
  
  printf("x: %d ", x);
  printf("y: %d\n", y);
  printf("dx: %d ", pp->delta_x);
  printf("dy: %d\n", pp->delta_y);
  switch (state) {
    case INIT:
      printf("INIT\n");
      only_lb_pressed = pp->lb && !pp->rb && !pp->mb;
      if (only_lb_pressed) {
        state = DRAW_LEFT;
        x = 0;
        y = 0;
      }
      break;

    case DRAW_LEFT:
      printf("DRAW_LEFT\n");
      x += pp->delta_x;
      y += pp->delta_y;
      
      only_lb_pressed = pp->lb && !pp->rb && !pp->mb;
      none_pressed = !pp->lb && !pp->rb && !pp->mb;
      correct_slope = abs(y) > x && y>0 && x>0;
      min_dist = x >= x_len;  // if none_pressed && x < x_len => go to INIT
      tolerant = (pp->delta_x < 0) && (abs(pp->delta_x) < tolerance) && (pp->delta_y < 0) && (abs(pp->delta_y) < tolerance);
      if(none_pressed && correct_slope && min_dist){
        state = VERTEX;
        break;
      }
      if (only_lb_pressed && tolerant) {
        break;
      }
      if (!(only_lb_pressed && correct_slope)) { 
        
        printf("Sou merda a desenhar o 1o traco\n");
        state = INIT;
        break;
      }
      break;

    case VERTEX:
      printf("VERTEX\n");
      x = 0;
      y = 0;
      x += pp->delta_x;
      y += pp->delta_y;

      if (pp->mb || (abs(x) > tolerance) || (abs(y) > tolerance)) {
        state = INIT;
        break;
      }
      
      if (pp->lb) {
        state = DRAW_LEFT;
        x = 0;
        y = 0;
        break;
      }
      
      if(pp->rb) {
        state = DRAW_RIGHT;
        x = 0;
        y = 0;
        break;
      }
      break;

    case DRAW_RIGHT:
      printf("DRAW_RIGHT\n");
      x += pp->delta_x;
      y += pp->delta_y;
      
      only_rb_pressed = !pp->lb && pp->rb && !pp->mb;
      none_pressed = !pp->lb && !pp->rb && !pp->mb;
      correct_slope = abs(y) > x && y<0 && x>0;
      min_dist = x >= x_len;  // if none_pressed && x < x_len => go to INIT
      tolerant = (pp->delta_x < 0) && (abs(pp->delta_x) < tolerance) && (pp->delta_y < 0) && (abs(pp->delta_y) < tolerance);
      if(none_pressed && min_dist && correct_slope) {
        state = FINAL;
        break;
      }
      if (only_rb_pressed && tolerant) {
        break;
      }
      if (!(only_rb_pressed && correct_slope)){
        printf("Sou merda a desenhar o 2o traco\n");
        state = INIT;
        break;
      }
      break;

    case FINAL:
      printf("FINAL\n");
    default:
      break;
  }
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {

  /*  Escrito no quadro
    1- Enable data reporting
    2- Subscrie mouse
    3- Ciclo prof -> ver eeventos do mouse
    4- Unsubscribe data reporting
  */
  
  uint8_t mouse_bit_no;

  if(mouse_subscribe_interrupts(&mouse_bit_no) != 0) return EXIT_FAILURE;

  if (mouse_disable_int() != 0) {
    printf("mouse_disable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_enable_data_report() != 0) {
    printf("mouse_enable_data_report inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (mouse_enable_int() != 0) {
    printf("mouse_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  
  int ipc_status;
  message msg;

  struct packet packet;
  uint8_t index = 0;

  while(state != FINAL){
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
            if (mouse_get_packet(&packet, &index, packet_byte) != 0) return EXIT_FAILURE;
            if (index == 3) {
              index = 0;
              mouse_print_packet(&packet);
              // struct mouse_ev *ev = mouse_detect_event(&packet);
              process_event(&packet, x_len, tolerance);
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
  
  if (mouse_disable_int() != 0) {
    printf("mouse_disable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_disable_data_report() != 0) {
    printf("mouse_disable_data_report inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (mouse_enable_int() != 0) {
    printf("mouse_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_unsubscribe_interrupts() != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
