#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t) val;

  return EXIT_SUCCESS;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t) (val>>8);
  return EXIT_SUCCESS;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t var;
  if (sys_inb(port, &var)!= 0) return EXIT_FAILURE;
  *value = var;
  return EXIT_SUCCESS;
}
