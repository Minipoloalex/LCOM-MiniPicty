#include "serial_port.h"

static uint16_t base_addr;

int (ser_set_base_addr)(uint16_t addr) {
  base_addr = addr;
  return EXIT_SUCCESS;
}

int (ser_read_line_control)(uint8_t *lcr) {
  if (util_sys_inb(base_addr + SER_LCR, lcr) != OK) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_write_line_control)(uint8_t lcr) {
  if (sys_outb(base_addr + SER_LCR, lcr) != OK) {
    printf("sys_outb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_read_divisor)(uint16_t *divisor, uint8_t *lcr) {
  if (lcr == NULL) {
    if (ser_read_line_control(lcr) != OK) {
      printf("ser_read_line_control() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  // if dlab is not set, we have to set it to access DLL and DLM
  if (!(*lcr & SER_LCR_DLAB)) {
    *lcr |= SER_LCR_DLAB;
    if (ser_write_line_control(*lcr) != OK) {
      printf("ser_write_line_control() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }

  uint8_t dll, dlm;
  if (util_sys_inb(base_addr + SER_DLL, &dll) != OK) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_sys_inb(base_addr + SER_DLM, &dlm) != OK) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  *divisor = (dlm << 8) | dll;

  return EXIT_SUCCESS;
}

int (ser_write_divisor)(uint16_t divisor, uint8_t *lcr) {
  if (lcr == NULL) {
    if (ser_read_line_control(lcr) != OK) {
      printf("ser_read_line_control() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  // if dlab is not set, we have to set it to access DLL and DLM
  if (!(*lcr & SER_LCR_DLAB)) {
    *lcr |= SER_LCR_DLAB;
    if (ser_write_line_control(*lcr) != OK) {
      printf("ser_write_line_control() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  uint8_t dll = (uint8_t) (divisor & 0xFF);
  uint8_t dlm = (uint8_t) ((divisor >> 8) & 0xFF);
  if (sys_outb(base_addr + SER_DLL, dll) != OK) {
    printf("sys_outb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (sys_outb(base_addr + SER_DLM, dlm) != OK) {
    printf("sys_outb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_read_int_enable)(uint8_t *ier, uint8_t *lcr) {
  if (lcr == NULL) {
    if (ser_read_line_control(lcr) != OK) {
      printf("ser_read_line_control() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  // if dlab is set, we need to unset it (set it to 0)
  if (*lcr & SER_LCR_DLAB) {
    *lcr &= ~SER_LCR_DLAB;
    if (ser_write_line_control(*lcr) != OK) {
      printf("ser_write_line_control() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }

  if (util_sys_inb(base_addr + SER_IER, ier) != OK) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
