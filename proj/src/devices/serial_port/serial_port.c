#include "serial_port.h"

/**
 * @brief Enumerates the possible receiving states of the serial port from the communication protocol.
 */
typedef enum {
  SLEEPING,
  RECEIVING_MOUSE_DRAWING,
  RECEIVING_MOUSE_NOT_DRAWING,
  RECEIVING_WORD_INDEX,
} ser_state_t;

static uint16_t base_addr;
static int hook_id;

static queue_t *transmitter_queue = NULL;
static queue_t *receiver_queue = NULL;

static ser_state_t ser_state = SLEEPING;

int ser_return_value = 0;

/* ========== Functions only used inside this module (serial port) ========= */
/**
 * @brief Sets the base address of the serial port and creates the queues.
 * @param addr base address of the serial port
 */
int (ser_set_base_addr)(uint16_t addr);
/**
 * @brief Reads the LCR - line control register
 * @param lcr pointer to the variable that will store the line control register value
 */
int (ser_read_line_control)(uint8_t *lcr);
/**
 * @brief Writes to the LCR - line control register
 * @param lcr value to be written to the line control register
 */
int (ser_write_line_control)(uint8_t lcr);
/**
 * @brief Sets the line control register of the serial port with the given parameters. These parameters are not passed blindly, but they are interpreted as given in Lab7.
 * @param word_length number of bits per character
 * @param stop_bit number of stop bits
 * @param parity parity of the serial port
 */
int (ser_set_line_config)(uint8_t word_length, uint8_t stop_bit, int8_t parity);
/**
 * @brief Reads the divisor latch register.
 * Reads the line control register and sets the DLAB bit to be able to access DLL and DLM (if it is not already set)
 * @param divisor pointer to the variable that will store the divisor
 * @param lcr pointer to the variable that may store the line control register (can be NULL and in that case it will be read and updated)
 * @return 0 if success, different than 0 otherwise
 */
int (ser_write_divisor)(uint16_t divisor);
/**
 * @brief Sets the baud rate of the serial port
 * @param rate baud rate of the serial port
 */
int (ser_set_baud_rate)(uint32_t rate);
/**
 * @brief Writes to the IER - interrupt enable register. This register is used to enable the interrupts of the serial port.
 * @param ier value to be written to the interrupt enable register
 */
int (ser_write_int_enable)(uint8_t ier);
/**
 * @brief Reads the LSR - line status register
 * @param status pointer to the variable that will store the line status register value
 */
int (ser_read_line_status)(uint8_t *status);
/**
 * @brief Reads data from the Receiver Buffer Register (RBR). Assumes that the data is ready to be read
 * @param data pointer to the variable that will store the data read from the RBR
 */
int (ser_read_data)(uint8_t *data);
/**
 * @brief Writes data to the Transmitter Holding Register (THR). Assumes that the data is ready to be written
 * @param data data to be written to the THR
 */
int (ser_write_data)(uint8_t data);
/**
 * @brief Writes the given configuration value to the control register of the FIFO
 * @param config configuration value to be written to the control register of the FIFO
 */
int (ser_write_fifo_control)(uint8_t config);
/**
 * @brief Reads the interrupt id, from the IIR (interrupt identification register)
 * @param id return value of the interrupt id
 */
int (ser_read_int_id)(uint8_t *id);
/**
 * @brief This functions assures that no byte sent to the serial port is equal to the special byte of the communication protocol
 * The solution used is that if the byte is equal to the end of transmission byte, it is replaced by a replacement byte (0xFF to 0xFE)
 */
int (replace_ser_special(uint8_t *data));
/**
 * @brief Writes the default configuration value to the control register of the FIFO. This is SER_FCR_DEFAULT.
 */
int (ser_write_fifo_control_default)();
/**
 * @brief Adds a byte to the transmitter queue that will later be communicated/sent to the serial port
 */
int (ser_add_byte_to_transmitter_queue)(uint8_t c);
/**
 * @brief Initializes the serial port communication parameters. They are specified as in the Lab7 handout.
 * @param base_addr base address of the serial port
 * @param baud_rate baud rate of the serial port
 * @param word_length number of bits per character
 * @param stop_bit number of stop bits
 * @param parity parity of the serial port
 */
int (ser_init_private)(uint16_t base_addr, uint32_t baud_rate, uint8_t word_length, uint8_t stop_bit, int8_t parity);
/* =========================================================================================== */

int (ser_init)() {
  if (ser_init_private(SER_COM1, SER_MAX_BITRATE, 8, 1, 1) != OK) {
    printf("ser_init_private() inside %s failed\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int (ser_init_private)(uint16_t base_addr, uint32_t baud_rate, uint8_t word_length, uint8_t stop_bit, int8_t parity){
  if (ser_set_base_addr(base_addr) != EXIT_SUCCESS) {
    printf("ser_set_base_addr() inside %s failed", __func__);
    return EXIT_FAILURE;
  }
  if (ser_set_baud_rate(baud_rate) != EXIT_SUCCESS) {
    printf("ser_set_baud_rate() inside %s failed", __func__);
    return EXIT_FAILURE;
  }
  if (ser_set_line_config(word_length, stop_bit, parity) != EXIT_SUCCESS) {
    printf("ser_set_line_config() inside %s failed", __func__);
    return EXIT_FAILURE;
  }
  if (ser_write_fifo_control_default()) {
    printf("ser_write_fifo_control_default() inside %s failed", __func__);
    return EXIT_FAILURE;
  }
  if (ser_write_int_enable(SER_IER_RDA | SER_IER_RLS | SER_IER_THRE) != EXIT_SUCCESS) {
    printf("ser_write_int_enable() inside %s failed", __func__);
    return EXIT_FAILURE;
  }
  if (ser_read_from_fifo() != OK) {
    printf("ser_read_from_fifo() inside %s failed", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
void (delete_ser)() {
  delete_queue(transmitter_queue);
  delete_queue(receiver_queue);
}

int(ser_set_base_addr)(uint16_t addr) {
  transmitter_queue = create_queue(QUEUE_SIZE, sizeof(uint8_t));
  if (transmitter_queue == NULL) {
    printf("Error creating transmitter queue inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  receiver_queue = create_queue(QUEUE_SIZE, sizeof(uint8_t));
  if (receiver_queue == NULL) {
    printf("Error creating receiver queue inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  base_addr = addr;
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
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (ser_add_byte_to_receiver_queue)(uint8_t byte) {
  if (push_queue(receiver_queue, &byte) != OK) {
  }
  return EXIT_SUCCESS;
}

int (ser_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    printf("bit_no is NULL inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  *bit_no = hook_id;
  return sys_irqsetpolicy(SER_COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
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
  uint32_t divisor = SER_MAX_BITRATE / rate;
  if (ser_write_divisor(divisor) != OK) {
      printf("Error writing divisor: ser_write_divisor() inside %s\n", __func__);
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
      return EXIT_SUCCESS;
    }
    if (pop_queue(transmitter_queue, &data) != OK) return EXIT_FAILURE;

    if (ser_write_data(data) != OK) {
      printf("ser_write_data() inside %s\n", __func__);
      return EXIT_FAILURE;
    }
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
  uint8_t lsr;
  if (!(iir & SER_IIR_INT_NOT_PEND)) {  /* interrupt pending */
    switch ((iir & SER_IIR_INT_ID) >> SER_IIR_INT_ID_POSITION) {
      case SER_IIR_INT_ID_RDA:      // data ready
        if (ser_read_from_fifo() != OK) {
          printf("ser_read_from_fifo() inside %s\n", __func__);
          ser_return_value = EXIT_FAILURE;
          return;
        }
        break;
      case SER_IIR_INT_ID_THRE:   // transmitter empty
        if (ser_write_to_fifo() != OK) {
          printf("ser_write_to_fifo() inside %s\n", __func__);
          ser_return_value = EXIT_FAILURE;
          return;
        }
        break;
      case SER_IIR_INT_ID_CTI:    // character timeout
        if (ser_read_from_fifo() != OK) {
          printf("ser_read_from_fifo() inside %s\n", __func__);
          ser_return_value = EXIT_FAILURE;
          return;
        }
        break;
      case SER_IIR_INT_ID_LS:     // error interrupt: Line status
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
      default:
        printf("Unknown interrupt\n");
        ser_return_value = EXIT_FAILURE;
        return;
    }
    ser_return_value = EXIT_SUCCESS;
    return;
  }
  ser_return_value = EXIT_FAILURE;
}

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

int (replace_ser_special(uint8_t *data)) {
  if (*data == SER_SPECIAL) {
    *data = SER_SPECIAL_REPLACEMENT;
  }
  return EXIT_SUCCESS;
}
int (ser_add_position_to_transmitter_queue)(drawing_position_t drawing_position) {
  uint8_t mouse_pos_bytes[4];
  util_get_LSB(drawing_position.position.x, &mouse_pos_bytes[0]);
  util_get_MSB(drawing_position.position.x, &mouse_pos_bytes[1]);
  util_get_LSB(drawing_position.position.y, &mouse_pos_bytes[2]);
  util_get_MSB(drawing_position.position.y, &mouse_pos_bytes[3]);
  for (int i = 0; i < 4; i++) {
    if (replace_ser_special(&mouse_pos_bytes[i])) return EXIT_FAILURE;
  }
  if (ser_add_byte_to_transmitter_queue(SER_SPECIAL)) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(drawing_position.is_drawing ? SER_MOUSE_DRAWING : SER_MOUSE_NOT_DRAWING)) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(mouse_pos_bytes[0])) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(mouse_pos_bytes[1])) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(mouse_pos_bytes[2])) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(mouse_pos_bytes[3])) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(SER_SPECIAL)) return EXIT_FAILURE;
  if (ser_write_to_fifo()) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
int (ser_add_button_click_to_transmitter_queue)(uint8_t index) {
  if (ser_add_byte_to_transmitter_queue(SER_SPECIAL)) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue('A' + index)) return EXIT_FAILURE;
  if (ser_write_to_fifo()) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
int (ser_add_word_index)(uint8_t index) {
  if (ser_add_byte_to_transmitter_queue(SER_SPECIAL)) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(SER_WORD_INDEX)) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(index)) return EXIT_FAILURE;
  if (ser_write_to_fifo()) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
int (ser_add_won_round)() {
  if (ser_add_byte_to_transmitter_queue(SER_SPECIAL)) return EXIT_FAILURE;
  if (ser_add_byte_to_transmitter_queue(SER_WON_ROUND)) return EXIT_FAILURE;
  if (ser_write_to_fifo()) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int (ser_read_bytes_from_receiver_queue)(player_drawer_t *drawer, state_t *app_state, bool *won_round) {
  uint8_t byte;
  static uint8_t bytes[4];
  static uint8_t byte_index = 0;
  player_t *player = NULL;
  if (drawer != NULL) {
    player = player_drawer_get_player(drawer);
  }
  buttons_array_t *buttons = NULL;
  while (!is_empty_queue(receiver_queue)) {
    if (pop_queue(receiver_queue, &byte) != OK) {
      printf("pop_queue() inside %s failed\n", __func__);
      return EXIT_FAILURE;
    }
    switch (ser_state) {
      case SLEEPING:
        switch (byte) {
          case SER_SPECIAL:
            break;
          case SER_MOUSE_DRAWING:
            ser_state = RECEIVING_MOUSE_DRAWING;
            break;
          case SER_MOUSE_NOT_DRAWING:
            ser_state = RECEIVING_MOUSE_NOT_DRAWING;
            break;
          case SER_WORD_INDEX:
            ser_state = RECEIVING_WORD_INDEX;
            break;
          case SER_WON_ROUND:
            if (won_round == NULL) {
              printf("won_round is null inside %s\n", __func__);
              continue;
            }
            *won_round = true;
            break;
          default:
            buttons = app_state->get_buttons(app_state);
            if (buttons == NULL) {
              printf("buttons are null inside %s\n", __func__);
              continue;
            }
            if (byte >= SER_FIRST_BUTTON && byte < SER_FIRST_BUTTON + buttons->num_buttons){
              // case SER_BUTTON_INDEX
              button_t *clicked_button = buttons->buttons[byte - SER_FIRST_BUTTON];
              clicked_button->onClick(clicked_button);
            }
            break;
        }
        break;
      case RECEIVING_MOUSE_DRAWING: case RECEIVING_MOUSE_NOT_DRAWING:
        if (player == NULL) {
          ser_state = SLEEPING;
          byte_index = 0;
          continue;
        }
        if (byte == SER_SPECIAL) {
          if (byte_index != 4) {  // got the end byte before the 4 bytes of the position
            ser_state = SLEEPING;
            byte_index = 0;
            printf("lost some bytes\n");
            continue;
          }
          position_t position;
          if (get_position(bytes, &position)) return EXIT_FAILURE;
          drawing_position_t drawing_position = {
            .position = position,
            .is_drawing = ser_state == RECEIVING_MOUSE_DRAWING
          };
          if (player_add_next_position(player, &drawing_position)) {
            printf("player_add_next_position() inside %s failed\n", __func__);
            return EXIT_FAILURE;
          }
          ser_state = SLEEPING;
          byte_index = 0;
          break;
        }
        bytes[byte_index++] = byte;
        break;
      case RECEIVING_WORD_INDEX:
        if (byte == SER_SPECIAL) {
          ser_state = SLEEPING;
          printf("lost some bytes inside ser_state RECEIVING_WORD_INDEX\n");
          continue;
        }
        app_state->word_index = byte;
        ser_state = SLEEPING;
        break;

      default:
        break;
    }
  }
  return EXIT_SUCCESS;
}
