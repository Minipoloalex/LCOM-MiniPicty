#include <lcom/lcf.h>

#include <stdint.h>
#include "serial_port.h"
#include "test7.h"
#include "queue.h"


int main(int argc, char* argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

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

  
  // printf("\n");
  // printf("testing ser_test_poll()\n");  

  // if (!is_transmitter) {
  //   if (ser_test_int(base_addr, is_transmitter, bits_per_char, stop_bits, parity, rate, stringc, strings) != OK) {
  //     printf("Error in ser_test_int()\n");
  //     return EXIT_FAILURE;
  //   }
  // }
  // else {
  //   if (ser_test_poll(base_addr, is_transmitter, bits_per_char, stop_bits, parity, rate, stringc, strings) != OK) {
  //   printf("Error in ser_test_poll()\n");
  //   return EXIT_FAILURE;
  // }  
  // }
  // if (ser_test_poll(base_addr, is_transmitter, bits_per_char, stop_bits, parity, rate, stringc, strings) != OK) {
  //   printf("Error in ser_test_poll()\n");
  //   return EXIT_FAILURE;
  // }

  // if (ser_test_int(base_addr, is_transmitter, bits_per_char, stop_bits, parity, rate, stringc, strings) != OK) {
  //   printf("Error in ser_test_int()\n");
  //   return EXIT_FAILURE;
  // }

  if (ser_test_fifo(base_addr, is_transmitter, bits_per_char, stop_bits, parity, rate, stringc, strings) != OK) {
    printf("Error in ser_test_fifo()\n");
    return EXIT_FAILURE;
  }

  printf("proj_main_loop() ended successfully\n");
  return EXIT_SUCCESS;
}
