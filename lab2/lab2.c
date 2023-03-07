#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t config = 0;

  if (timer_get_conf(timer, &config) != 0) return EXIT_FAILURE;
  if (timer_display_conf(timer, config, field) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  if (freq == 0) return EXIT_FAILURE;
  if (timer_set_frequency(timer, freq) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

extern int counter;
int(timer_test_int)(uint8_t time) {
  uint8_t bit_no_timer;

  /*Subscribe*/
  if (timer_subscribe_int(&bit_no_timer) != 0) return EXIT_FAILURE;

  int ipc_status;
  message msg;

  while (time > 0){
    int r;
    if((r=driver_receive(ANY, &msg, &ipc_status)) != 0){
      printf("driver_receive failed with: %d",r);
      continue;
    }
  }

  if (is_ipc_notify(ipc_status)){
    switch(_ENDPOINT_P(msg.m_source)){
      case HARDWARE:
        if (msg.m_notify.interrupts & BIT(bit_no_timer)){
          timer_int_handler();

          if ((counter % (int) sys_hz()) == 0){
            timer_print_elapsed_time();
            time--;
          }
        }
        break;
      default:
        break;
    }
  }
  else{}

  if (timer_unsubscribe_int() != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
