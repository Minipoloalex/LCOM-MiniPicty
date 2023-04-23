#include "serial_port.h"

static uint16_t base_addr;
static int hook_id;
static uint8_t is_transmitter = 0;

static const uint8_t queue_size = QUEUE_SIZE;

queue_t *transmitter_queue = NULL;
queue_t *receiver_queue = NULL;

// uint8_t transmitter_queue[QUEUE_SIZE]; // can send 100 bytes
// uint8_t transmitter_processed_ind = 0;  // bytes that were already processed (written to THR)
// uint8_t write_ind = 0;
// uint8_t to_process_write = 0;

// uint8_t receiver_queue[QUEUE_SIZE];    // can receive 100 bytes
// uint8_t receiver_processed_ind = 0;     // bytes that were already processed (read from RBR)
// uint8_t read_ind = 0;
// uint8_t to_process_read = 0;

uint8_t c;
int ser_return_value = 0;


int(ser_set_base_addr)(uint16_t addr, uint8_t is_tr) {
  transmitter_queue = create_queue(queue_size, sizeof(uint8_t));
  if (transmitter_queue == NULL) {
    printf("Error creating transmitter queue inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  receiver_queue = create_queue(queue_size, sizeof(uint8_t));
  if (receiver_queue == NULL) {
    printf("Error creating receiver queue inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  base_addr = addr;
  is_transmitter = is_tr;
  switch (addr) {
    case SER_COM1:
      hook_id = SER_COM1_IRQ;
      break;
    case SER_COM2:
      hook_id = SER_COM2_IRQ;
      break;
    default: return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_add_byte_to_transmitter_queue)(uint8_t byte) {
  if (push_queue(transmitter_queue, &byte) != OK) {
    printf("push_queue() (queue is full) inside %s failed\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_add_byte_to_receiver_queue)(uint8_t byte) {
  if (push_queue(receiver_queue, &byte) != OK) {
    printf("push_queue() inside %s failed\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_read_bytes_from_receiver_queue)() {
  uint8_t byte;
  while (!is_empty_queue(receiver_queue)) {
    if (pop_queue(receiver_queue, &byte) != OK) {
      printf("pop_queue() inside %s failed\n", __func__);
      return EXIT_FAILURE;
    }
    printf("Byte read from receiver queue: %c\n", byte);
  }
  return EXIT_SUCCESS;
}


int (ser_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    printf("bit_no is NULL inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  *bit_no = hook_id;
  return sys_irqsetpolicy(*bit_no, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (ser_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int(ser_read_line_control)(uint8_t *lcr) {
  if (lcr == NULL) {
    printf("lcr is NULL inside %s\n", __func__);
    return EXIT_FAILURE;
  }
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
  if (divisor == NULL) {
    printf("divisor is NULL inside %s\n", __func__);
    return EXIT_FAILURE;
  }
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
  if (ier == NULL) {
    printf("ier is NULL inside %s\n", __func__);
    return EXIT_FAILURE;
  }
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
  if (status == NULL) {
    printf("status is NULL inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_sys_inb(base_addr + SER_LSR, status) != OK) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


int (ser_read_data)(uint8_t *data) {
  if (data == NULL) {
    printf("data is NULL inside %s\n", __func__);
    return EXIT_FAILURE;
  }
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

int (ser_read_char_int)(uint8_t *data) {
  if (data == NULL) {
    printf("Invalid pointer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (ser_read_data(data) != OK) {
    printf("ser_read_data() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(ser_read_char)(uint8_t *data) {
  if (data == NULL) {
    printf("Invalid pointer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
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

int (ser_write_char_int)(uint8_t data) {
  if (ser_write_data(data) != OK) {
    printf("ser_read_data() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
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

int (ser_read_int_id)(uint8_t *id) {
  if (id == NULL) {
    printf("Invalid pointer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (util_sys_inb(base_addr + SER_IIR, id)) {
    printf("util_sys_inb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * @brief 
 * This is the serial port interrupt handler. It should be called when an interrupt is received.
 * It should read the interrupt id and handle the interrupt accordingly.
 * This is for simple interrupts and not fifo interrupts. Might get changed later.
 */
void (ser_ih)() {
  uint8_t iir;
  if (ser_read_int_id(&iir) != OK) {
    printf("ser_read_int_id() inside %s\n", __func__);
    ser_return_value = EXIT_FAILURE;
    return;
  }
  printf("Interrupt id: %02x\n", iir);
  uint8_t ier;
  if (ser_read_int_enable(&ier) != OK) {
    printf("ser_read_int_enable() inside %s\n", __func__);
    ser_return_value = EXIT_FAILURE;
    return;
  }
  printf("ier: %02x\n", ier);
  uint8_t lsr;
  if (!(iir & SER_IIR_INT_NOT_PEND)) {  /* interrupt pending */
    switch ((iir & SER_IIR_INT_ID) >> SER_IIR_INT_ID_POSITION) {
      case SER_IIR_INT_ID_RDA:  // SER_IIR_RX_INT (data ready)
        printf("Received interrupt: can read another character\n");
        if (ser_read_char_int(&c) != OK) {
          ser_return_value = EXIT_FAILURE;
          return;
        }
        printf("Read a character: %c\n", c);
        if (ser_add_byte_to_receiver_queue(c) != OK) {
          ser_return_value = EXIT_FAILURE;
          return;
        }
        break;
      case SER_IIR_INT_ID_THRE:  // SER_IIR_TX_INT (transmitter empty)
        printf("Transmit interrupt: can write another character\n");
        if (pop_queue(transmitter_queue, &c) != OK) {
          printf("pop_queue() (queue is empty) inside %s\n", __func__);
          ser_return_value = EXIT_SUCCESS;
          return;
        }
        printf("Writing character %c\n", c);

        if (ser_write_char_int(c) != OK) {
          ser_return_value = EXIT_FAILURE;
          return;
        }
        // if (ser_read_int_id(&iir) != OK) {
        //   printf("ser_read_int_id() inside %s\n", __func__);
        //   ser_return_value = EXIT_FAILURE;
        //   return;
        // }
        // printf("Interrupt id: %02x\n", iir);
        break;
      case SER_IIR_INT_ID_LS:       // SER_IIR_RX_ERR (error interrupt: Line status)
        printf("Receive error interrupt\n");
        ser_return_value = EXIT_FAILURE;
        if (ser_read_line_status(&lsr) != OK) {
          printf("ser_read_line_status() inside %s\n", __func__);
          return;
        }
        if (lsr & SER_LSR_OE) {
          printf("Overrun Error inside %s: a character in RBR was overwritten by another\n", __func__);
          return;
        }
        if (lsr & SER_LSR_PE) {
          printf("Parity Error inside %s: received character does not have expected parity\n", __func__);
          return;
        }
        if (lsr & SER_LSR_FE) {
          printf("Framing Error inside %s: received character does not have expected stop bit\n", __func__);
          return;
        }
        return;
      // case SER_IIR_XXXX:            // SER_IIR_XXXX
      //   printf("XXXX interrupt\n");
      //   break;
      default:
        printf("Unknown interrupt\n");
        ser_return_value = EXIT_FAILURE;
        return;
    }
    ser_return_value = EXIT_SUCCESS;
    return;
  }
  printf("No interrupt pending inside %s\n", __func__);
  ser_return_value = EXIT_FAILURE;
}






/* FIFO Reading chars */

// void ser_ih() {
// sys_inb(ser_port + SER_IIR, &iir);
// if( iir & SER_INT_PEND ) {
// switch( iir & INT_ID ) {
// case SER_RX_INT:
// ... /* read received character */
// case SER_TX_INT:
// ... /* put character to sent */
// case SER_RX_ERR:
// ... /* notify upper level */
// case SER_XXXX:
// ... /* depends on XXX */
// }
// }
// }


/* FIFO Reading chars */
// sys_outb(ser_port + SER_FCR, 0x??); // Enable FIFOs
// sys_inb(ser_port + SER_IIR, &iir); // Check FIFO state
// void ser_ih() { // serial port IH
//   while( lsr & SER_RX_RDY ) { // Read all characters in FIFO
//   // check errors
//   util_sys_inb(ser_port + SER_DATA, &c);
//   // "process" character read
// sys_inb(ser_port + SER_LSR, &lsr);
// }
// void ser_ih() { // serial port IH
// ...
// while( !queue_is_full(qptr) && (lsr & SER_RX_RDY)) {
// ...
// }
int (ser_write_fifo_control_default)() {
  return ser_write_fifo_control(SER_FCR_DEFAULT);
}

int (ser_write_fifo_control)(uint8_t config) {
  if (sys_outb(base_addr + SER_FCR, config) != OK) {
    printf("sys_outb() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_read_from_fifo)() {
  uint8_t lsr;
  uint8_t data;
  if (ser_read_line_status(&lsr) != OK) {
    printf("ser_read_line_status() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  while( lsr & SER_LSR_DATA_READY ) { // Read all characters in FIFO
    if (ser_read_data(&data) != OK) {
      printf("ser_read_data() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (ser_add_byte_to_receiver_queue(data) != OK) {
      printf("ser_add_byte_to_receiver_queue() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (ser_read_line_status(&lsr) != OK) {
      printf("ser_read_line_status() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
int (ser_write_to_fifo)() {
  uint8_t lsr;
  uint8_t data;
  if (ser_read_line_status(&lsr) != OK) {
    printf("ser_read_line_status() inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  while (lsr & SER_LSR_THRE) {
    if (is_empty_queue(transmitter_queue)) {
      printf("transmitter queue empty inside %s\n", __func__);
      return EXIT_SUCCESS;
    }
    if (pop_queue(transmitter_queue, &data) != OK) return EXIT_FAILURE;

    if (ser_write_data(data) != OK) {
      printf("ser_write_data() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    printf("Character sent: %c\n", data);
    if (ser_read_line_status(&lsr) != OK) {
      printf("ser_read_line_status() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}


void (ser_ih_fifo)() {
  uint8_t iir;
  if (ser_read_int_id(&iir) != OK) {
    printf("ser_read_int_id() inside %s\n", __func__);
    ser_return_value = EXIT_FAILURE;
    return;
  }
  printf("Interrupt id: %02x\n", iir);
  uint8_t lsr;
  if (!(iir & SER_IIR_INT_NOT_PEND)) {  /* interrupt pending */
    switch ((iir & SER_IIR_INT_ID) >> SER_IIR_INT_ID_POSITION) {
      case SER_IIR_INT_ID_RDA:  // SER_IIR_RX_INT (data ready)
        printf("Received interrupt RDA\n");
        if (ser_read_from_fifo() != OK) {
          printf("ser_read_from_fifo() inside %s\n", __func__);
          return;
        }
        break;
      case SER_IIR_INT_ID_THRE:  // SER_IIR_TX_INT (transmitter empty)
        printf("Transmit interrupt THRE\n");
        if (ser_write_to_fifo() != OK) {
          printf("ser_write_to_fifo() inside %s\n", __func__);
          return;
        }
        break;
      case SER_IIR_INT_ID_LS:       // SER_IIR_RX_ERR (error interrupt: Line status)
        printf("Receive error interrupt\n");
        ser_return_value = EXIT_FAILURE;
        if (ser_read_line_status(&lsr) != OK) {
          printf("ser_read_line_status() inside %s\n", __func__);
          return;
        }
        if (lsr & SER_LSR_OE) {
          printf("Overrun Error inside %s: a character in RBR was overwritten by another\n", __func__);
          return;
        }
        if (lsr & SER_LSR_PE) {
          printf("Parity Error inside %s: received character does not have expected parity\n", __func__);
          return;
        }
        if (lsr & SER_LSR_FE) {
          printf("Framing Error inside %s: received character does not have expected stop bit\n", __func__);
          return;
        }
        return;
      // case SER_IIR_XXXX:            // SER_IIR_XXXX
      //   printf("XXXX interrupt\n");
      //   break;
      default:
        printf("Unknown interrupt\n");
        ser_return_value = EXIT_FAILURE;
        return;
    }
    ser_return_value = EXIT_SUCCESS;
    return;
  }
  printf("No interrupt pending inside %s\n", __func__);
  ser_return_value = EXIT_FAILURE;
}
