#include <lcom/lcf.h>

#include <stdint.h>
#include "serial_port.h"
#include "test7.h"

int main(int argc, char* argv[]) {
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

int (proj_main_loop)(int argc, char *argv[]) {
  printf("testing ser_test_conf()\n");
  unsigned short base_addr = SER_COM1;
  if (ser_test_conf(base_addr) != OK) {
    printf("Error in ser_test_conf()\n");
    return EXIT_FAILURE;
  }
  printf("testing ser_test_set()\n");
  uint8_t bits_per_char = 8;
  uint8_t stop_bits = 1;
  int8_t parity = -1;
  uint16_t rate = 1200;
  if (ser_test_set(base_addr, bits_per_char, stop_bits, parity, rate) != OK) {
    printf("Error in ser_test_set()\n");
    return EXIT_FAILURE;
  }
  

  return EXIT_SUCCESS;
}
