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
  uint32_t rate = 1200;
  if (ser_test_set(base_addr, bits_per_char, stop_bits, parity, rate) != OK) {
    printf("Error in ser_test_set()\n");
    return EXIT_FAILURE;
  }
  
  // printf("\n");
  // printf("testing ser_test_poll()\n");
  
  // char *strings[] = {"Hello", "World"};   // must not have dots (dot is the termination symbol)
  // int stringc = 2;
  // uint8_t is_transmitter = 1; // it is the receiver only if this value is 0
  // // if it is the transmitter, it will send the strings
  // // sends strings and a space ' ' in between them
  // // dot to terminate
  
  // // as a receiver, it should receive the strings and display them, until it receives a dot (should also be displayed)
  // if (ser_test_poll(base_addr, is_transmitter, bits_per_char, stop_bits, parity, rate, stringc, strings) != OK) {
  //   printf("Error in ser_test_poll()\n");
  //   return EXIT_FAILURE;
  // }
  printf("proj_main_loop() ended successfully\n");
  return EXIT_SUCCESS;
}
