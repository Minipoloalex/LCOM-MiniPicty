#include "serial_port.h"

static uint16_t base_addr;

int(ser_set_base_addr)(uint16_t addr) {
  base_addr = addr;
  return EXIT_SUCCESS;
}

int(ser_read_line_control)(uint8_t *lcr) {
  if (util_sys_inb(base_addr + SER_LCR, lcr) != OK) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_write_line_control)(uint8_t lcr) {
  if (sys_outb(base_addr + SER_LCR, lcr) != OK) {
    printf("sys_outb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int (ser_set_line_config)(uint8_t word_length, uint8_t stop_bit, int8_t parity) {
  uint8_t lcr;
  if (ser_read_line_control(&lcr) != OK) {
    printf("ser_read_line_control() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  
  lcr &= ~(SER_LCR_BITS_PER_CHAR | SER_LCR_STOP_BIT | SER_LCR_PARITY_CTRL);
    switch (parity) {
        case -1:
            lcr |= SER_LCR_PARITY_NONE;
            break;
        case 0:
            lcr |= SER_LCR_PARITY_EVEN;
            break;
        case 1:
            lcr |= SER_LCR_PARITY_ODD;
            break;
        default:
            printf("Invalid parity value: %d\n", parity);
            return EXIT_FAILURE;
    }
    switch (word_length) {
        case 5:
            lcr |= SER_LCR_5_BITS_PER_CHAR;
            break;
        case 6:
            lcr |= SER_LCR_6_BITS_PER_CHAR;
            break;
        case 7:
            lcr |= SER_LCR_7_BITS_PER_CHAR;
            break;
        case 8:
            lcr |= SER_LCR_8_BITS_PER_CHAR;
            break;
        default:
            printf("Invalid number of bits per character: %d\n", word_length);
            return EXIT_FAILURE;
    }
    switch (stop_bit) {
        case 1:
            lcr |= SER_LCR_1_STOP_BIT;
            break;
        case 2:
            lcr |= SER_LCR_2_STOP_BIT;
            break;
        default:
            printf("Invalid number of stop bits: %d\n", stop_bit);
            return EXIT_FAILURE;
    }
  
  if (ser_write_line_control(lcr) != OK) {
    printf("ser_write_line_control() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_read_divisor)(uint16_t *divisor) {
  uint8_t lcr;
  if (ser_read_line_control(&lcr) != OK) {
    printf("ser_read_line_control() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  
  // if dlab is not set, we have to set it to access DLL and DLM
  if (!(lcr & SER_LCR_DLAB)) {
    if (ser_write_line_control(lcr | SER_LCR_DLAB) != OK) {
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

int(ser_write_divisor)(uint16_t divisor) {
  uint8_t lcr;  
  if (ser_read_line_control(&lcr) != OK) {
    printf("ser_read_line_control() inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  // if dlab is not set, we have to set it to access DLL and DLM
  if (!(lcr & SER_LCR_DLAB)) {
    lcr |= SER_LCR_DLAB;
    if (ser_write_line_control(lcr) != OK) {
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

int (ser_set_baud_rate)(uint32_t rate) {
  if (rate > SER_MAX_BITRATE) {
    printf("Invalid baud rate: %d\n", rate);
    return EXIT_FAILURE;
  }
  printf("Setting baud rate to %d\n", rate);
  uint32_t divisor = SER_MAX_BITRATE / rate;
  printf("Setting divisor to %d\n", divisor);
  if (ser_write_divisor(divisor) != OK) {
      printf("Error writing divisor: ser_write_divisor() inside %s\n", __func__);
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_read_int_enable)(uint8_t *ier) {
  uint8_t lcr;
  if (ser_read_line_control(&lcr) != OK) {
    printf("ser_read_line_control() inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  // if dlab is set, we need to unset it (set it to 0)
  if (lcr & SER_LCR_DLAB) {
    if (ser_write_line_control(lcr & ~SER_LCR_DLAB) != OK) {
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

int (ser_write_int_enable)(uint8_t ier) {
  uint8_t lcr;
  if (ser_read_line_control(&lcr) != OK) {
    printf("ser_read_line_control() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
    // if dlab is set, we need to unset it (set it to 0)
  if (lcr & SER_LCR_DLAB) {
    if (ser_write_line_control(lcr & ~SER_LCR_DLAB) != OK) {
      printf("ser_write_line_control() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  if (sys_outb(base_addr + SER_IER, ier) != OK) {
    printf("sys_outb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_read_line_status)(uint8_t *status) {
  if (util_sys_inb(base_addr + SER_LSR, status) != OK) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


int (ser_read_data)(uint8_t *data) {
  if (util_sys_inb(base_addr + SER_RBR, data) != OK) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_write_data)(uint8_t data) {
  if (sys_outb(base_addr + SER_THR, data) != OK) {
    printf("sys_outb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_read_char)(uint8_t *data) {
  uint8_t status;
  if (ser_read_line_status(&status) != OK) {
    printf("ser_read_line_status() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (status & SER_LSR_DATA_READY) {  /* ready to be read */
    if (ser_read_data(data) != OK) {
      printf("ser_read_data() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (status & SER_LSR_OE) {
      printf("Overrun Error inside %s: a character in RBR was overwritten by another\n", __func__);
      return EXIT_FAILURE;
    }
    if (status & SER_LSR_PE) {
      printf("Parity Error inside %s: received character does not have expected parity\n", __func__);
      return EXIT_FAILURE;
    }
    if (status & SER_LSR_FE) {
      printf("Framing Error inside %s: received character does not have expected stop bit\n", __func__);
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int (ser_write_char)(uint8_t c) {
  uint8_t status;
  if (ser_read_line_status(&status) != OK) {
    printf("ser_read_line_status() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (status & SER_LSR_THRE) {  /* ready to be written */
    if (ser_write_data(c) != OK) {
      printf("ser_write_data() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}
