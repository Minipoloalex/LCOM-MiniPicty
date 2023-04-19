#include "test7.h"
#include "serial_port.h"
#include "i8042.h"

int (ser_test_conf)(unsigned short base_addr) {
    ser_set_base_addr(base_addr, 0);
    uint8_t lcr, ier;
    uint16_t divisor;
    if (ser_read_line_control(&lcr)) {
        printf("Error reading LCR\n");
        return 1;
    }
    if (ser_read_int_enable(&ier)) {
        printf("Error reading IER\n");
        return 1;
    }
    if (ser_read_divisor(&divisor)) {
        printf("Error reading divisor\n");
        return 1;
    }
    
    printf("LCR: 0x%02x\n", lcr);
    printf("IER: 0x%02x\n", ier);
    printf("Divisor: 0x%04x\n", divisor);
    
    // LCR fields
    uint8_t n_bits_per_char;
    switch (lcr & SER_LCR_BITS_PER_CHAR) {  // switch case equivalent to (lcr & SER_LCR_BITS_PER_CHAR) + 5
        case SER_LCR_5_BITS_PER_CHAR:
            n_bits_per_char = 5;
            break;
        case SER_LCR_6_BITS_PER_CHAR:
            n_bits_per_char = 6;
            break;
        case SER_LCR_7_BITS_PER_CHAR:
            n_bits_per_char = 7;
            break;
        case SER_LCR_8_BITS_PER_CHAR:
            n_bits_per_char = 8;
            break;
    }

    uint8_t n_stop_bits = (lcr & SER_LCR_STOP_BIT) ? 2 : 1; // case 0: 1 stop bit, case 1: 2 stop bits
    
    uint8_t parity = (lcr & SER_LCR_PARITY_CTRL);
    char *parity_str;
    switch (parity) {
        case SER_LCR_PARITY_NONE:
            parity_str = "none";
            break;
        case SER_LCR_PARITY_ODD:
            parity_str = "odd";
            break;
        case SER_LCR_PARITY_EVEN:
            parity_str = "even";
            break;
        case SER_LCR_PARITY_1:
            parity_str = "mark (always 1)";
            break;
        case SER_LCR_PARITY_0:
            parity_str = "space (always 0)";
            break;
    }
    uint8_t break_control = (lcr & SER_LCR_BREAK_CTRL) ? 1 : 0; // case 0: break control disabled, case 1: break control enabled
    uint8_t dlab = (lcr & SER_LCR_DLAB) ? 1 : 0; // case 0: DLAB disabled, case 1: DLAB enabled

    printf("Number of bits per character: %d\n", n_bits_per_char);
    printf("Number of stop bits: %d\n", n_stop_bits);
    printf("Parity: %s\n", parity_str);
    printf("Break control: %d\n", break_control);
    printf("DLAB: %d\n", dlab);
    

    // IER fields
    uint8_t rda_interrupt = (ier & SER_IER_RDA) ? 1 : 0;  // case 0: RDA interrupt disabled, case 1: RDA interrupt enabled
    uint8_t thre_interrupt = (ier & SER_IER_THRE) ? 1 : 0;  // case 0: THRE interrupt disabled, case 1: THRE interrupt enabled
    uint8_t rls_interrupt = (ier & SER_IER_RLS) ? 1 : 0;  // case 0: RLS interrupt disabled, case 1: RLS interrupt enabled
    uint8_t ms_interrupt = (ier & SER_IER_MS) ? 1 : 0;  // case 0: MS interrupt disabled, case 1: MS interrupt enabled

    printf("RDA interrupt: %d\n", rda_interrupt);
    printf("THRE interrupt: %d\n", thre_interrupt);
    printf("RLS interrupt: %d\n", rls_interrupt);
    printf("MS interrupt: %d\n", ms_interrupt);


    /* DL - Divisor latch (bitrate) */
    uint32_t bitrate = SER_MAX_BITRATE / divisor;
    printf("Bitrate: %d\n", bitrate);

    if (ser_write_line_control(lcr)) {
        printf("Error writing lcr: ser_write_line_control() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    // uint8_t lcr, ier, iir, fcr, mcr, lsr, msr, scr;
    // uint16_t dll, dlm;
    // int r;
    // if ((r = ser_read_line_control(base_addr, &lcr)) != 0)
    //     return r;
    // if ((r = ser_read_int_enable(base_addr, &ier)) != 0)
    //     return r;
    // if ((r = ser_read_int_id_fifo(base_addr, &iir)) != 0)
    //     return r;
    // if ((r = ser_read_fifo(base_addr, &fcr)) != 0)
    //     return r;
    // if ((r = ser_read_modem_control(base_addr, &mcr)) != 0)
    //     return r;
    // if ((r = ser_read_line_status(base_addr, &lsr)) != 0)
    //     return r;
    // if ((r = ser_read_modem_status(base_addr, &msr)) != 0)
    //     return r;
    // if ((r = ser_read_scratch(base_addr, &scr)) != 0)
    //     return r;
    // if ((r = ser_read_divisor(base_addr, &dll)) != 0)
    //     return r;
    // if ((r = ser_read_divisor(base_addr, &dlm)) != 0)
    //     return r;
    return EXIT_SUCCESS;
}


int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop, 
	           long parity, unsigned long rate) { 
    ser_set_base_addr(base_addr, 0);
    
    if (ser_set_baud_rate(rate)) {
        printf("Error setting baud rate: ser_set_baud_rate() inside %s\n", __func__);
        return EXIT_FAILURE;
    }

    if (ser_set_line_config(bits, stop, parity)) {
        printf("Error setting line config: ser_set_line_config() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    // lcr &= ~(SER_LCR_BITS_PER_CHAR | SER_LCR_STOP_BIT | SER_LCR_PARITY_CTRL);
    // switch (parity) {
    //     case -1:
    //         lcr |= SER_LCR_PARITY_NONE;
    //         break;
    //     case 0:
    //         lcr |= SER_LCR_PARITY_EVEN;
    //         break;
    //     case 1:
    //         lcr |= SER_LCR_PARITY_ODD;
    //         break;
    //     default:
    //         printf("Invalid parity value: %d\n", parity);
    //         return EXIT_FAILURE;
    // }
    // switch (bits) {
    //     case 5:
    //         lcr |= SER_LCR_5_BITS_PER_CHAR;
    //         break;
    //     case 6:
    //         lcr |= SER_LCR_6_BITS_PER_CHAR;
    //         break;
    //     case 7:
    //         lcr |= SER_LCR_7_BITS_PER_CHAR;
    //         break;
    //     case 8:
    //         lcr |= SER_LCR_8_BITS_PER_CHAR;
    //         break;
    //     default:
    //         printf("Invalid number of bits per character: %d\n", bits);
    //         return EXIT_FAILURE;
    // }
    // switch (stop) {
    //     case 1:
    //         lcr |= SER_LCR_1_STOP_BIT;
    //         break;
    //     case 2:
    //         lcr |= SER_LCR_2_STOP_BIT;
    //         break;
    //     default:
    //         printf("Invalid number of stop bits: %d\n", stop);
    //         return EXIT_FAILURE;
    // }

    // if (ser_write_line_control(lcr)) {
    //     printf("Error writing lcr: ser_write_line_control() inside %s\n", __func__);
    //     return EXIT_FAILURE;
    // }
    
    ser_test_conf(base_addr);
    return EXIT_SUCCESS;
}

int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, 
                    unsigned long stop, long parity, unsigned long rate, 
                    int stringc, char *strings[]) {
    /*
    Importante notes:
    - When the UART is operating in polled mode, the device driver must continually check the LSR to detect whether a relevant event such as receiving a character has occured. 
    - In the case of the transmitter, it must check the LSR for the THRE bit (Transmitter Holding Register Empty): checks if can send another byte
    */

    ser_set_base_addr(base_addr, tx);
    uint8_t initial_ier;
    if (ser_read_int_enable(&initial_ier) != OK) {
        printf("Error reading ier: ser_read_int_enable() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    
    if (ser_test_set(base_addr, bits, stop, parity, rate) != OK) {
        printf("Error setting serial port: ser_test_set() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    if (ser_write_int_enable(initial_ier & ~(SER_IER_RDA | SER_IER_THRE | SER_IER_RLS)) != OK) {
        printf("Error enabling interrupts: ser_enable_int() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    if (tx == 0) {  /* receiver */
        while(true) {
            uint8_t character;
            for (int i = 0; i < 500; i++) {
                /* read a character from the receiver buffer */
                if (ser_read_char(&character) == OK) {
                    break;  /* got a valid char (multiple attempts may be needed - polling) */
                }
                // printf("Error reading character: ser_read_char() inside %s\n", __func__);
                tickdelay(micros_to_ticks(20000));
            }
            printf("Character read: %c\n", character);
            if (character == '.') break;
        }
    }
    else {  /* transmiter */
        for (int i = 0; i < stringc; i++) {
            char *string = strings[i];
            for (int j = 0; string[j] != '\0'; j++) {
                char character = string[j];
                for (int k = 0; k < MAX_ATTEMPTS; k++) {
                    /* write a character to the transmitter buffer */
                    if (ser_write_char(character) == OK) {
                        break;  /* wrote a valid char (multiple attempts may be needed - polling) */
                    }
                    printf("Error writing character: ser_write_char() inside %s\n", __func__);
                }
                printf("Character written: %c\n", character);
            }

            if (i != stringc - 1) {
                for (int k = 0; k < MAX_ATTEMPTS; k++) {
                    /* write a character to the transmitter buffer */
                    if (ser_write_char(' ') == OK) {
                        break;  /* wrote a valid char (multiple attempts may be needed - polling) */
                    }
                    printf("Error writing character: ser_write_char() inside %s\n", __func__);
                }
                printf("Character written: %c\n", ' ');
            }
        }

        for (int k = 0; k < MAX_ATTEMPTS; k++) {
            /* write a character to the transmitter buffer */
            if (ser_write_char('.') == OK) {
                break;  /* wrote a valid char (multiple attempts may be needed - polling) */
            }
            printf("Error writing character: ser_write_char() inside %s\n", __func__);
        }
        printf("Character written: %c\n", '.');
    }

    if (ser_write_int_enable(initial_ier) != OK) {
        printf("Error enabling interrupts: ser_enable_int() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int ser_test_int(unsigned short base_addr, unsigned char tx, unsigned long bits, 
                    unsigned long stop, long parity, unsigned long rate, 
                    int stringc, char *strings[]) { 
    if (ser_set_base_addr(base_addr, tx) != OK) {
        printf("Error setting base address: ser_set_base_addr() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    if (ser_test_set(base_addr, bits, stop, parity, rate) != OK) {
        printf("Error setting serial port: ser_test_set() inside %s\n", __func__);
        return EXIT_FAILURE;
    }

    uint8_t iir;
    if (ser_read_int_id(&iir) != OK) {
        printf("Error reading iir: ser_read_int_id() inside %s\n", __func__);
        return EXIT_FAILURE;
    };
    printf("iir: %02x\n", iir);

    uint8_t initial_ier;
    if (ser_read_int_enable(&initial_ier) != OK) {
        printf("Error reading ier: ser_read_int_enable() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    printf("initial_ier: %02x\n", initial_ier);
    // subscribe interrupts
    uint8_t ser_bit_no;
    if (ser_subscribe_int(&ser_bit_no) != OK) {
        printf("Error subscribing interrupts: ser_subscribe_int() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    printf("ser_bit_no: %02x\n", ser_bit_no);
    int r, ipc_status;
    message msg;
    extern uint8_t c;
    extern int ser_return_value;
    do { // while the message hasn't been fully delivered
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with %d", r);
      }
      if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & BIT(ser_bit_no)) {
                printf("Processing interrupts\n");
                ser_ih();
                if (ser_return_value != OK) {
                    printf("ser_ih() returned error: %d inside %s\n", ser_return_value, __func__);
                    continue;
                }
                printf("Char read: c = %c\n", c);
            }
        }
      }
    } while (c != '.');
    if (ser_unsubscribe_int() != OK) {
        printf("Error unsubscribing interrupts: ser_unsubscribe_int() inside %s\n", __func__);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int ser_test_fifo(/* details to be provided */) {
    /* To be completed */
    return 0;
}
