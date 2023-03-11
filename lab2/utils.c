#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
<<<<<<< HEAD
  *lsb = (uint8_t) val;
  return EXIT_SUCCESS;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val >> 8);
  return EXIT_SUCCESS;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t var;
  #ifdef LAB3
  extern uint32_t cnt_sysinb;
  cnt_sysinb++;
  #endif
  if (sys_inb(port, &var) != 0) return EXIT_FAILURE;
  *value = (uint8_t) var;
  return EXIT_SUCCESS;
=======
  (*lsb) = (uint8_t) val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  (*msb) = (uint8_t) (val >> 8);
  return 0;
}

int(util_sys_inb)(int port, uint8_t *value) {
  uint32_t temp;
  if(sys_inb(port, &temp)){
      printf("Error on sys_inb() called by util_sys_inb()");
      return 1;
  }
  (*value) = (uint8_t) temp;
  return 0;
>>>>>>> francisco_ana
}
