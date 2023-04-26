#include "test7.h"
#include "serial_port.h"
#include "i8042.h"

int ser_test_fifo(unsigned short base_addr, unsigned char tx, unsigned long bits, 
                    unsigned long stop, long parity, unsigned long rate, 
                    int stringc, char *strings[]) {
    if (ser_init(base_addr, rate, bits, stop, parity) != OK) {
        printf("Error initializing serial port: ser_init() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    uint8_t ser_bit_no;
    if (ser_subscribe_int(&ser_bit_no) != OK) {
        printf("Error subscribing interrupts: ser_subscribe_int() inside %s\n", __func__);
        return EXIT_FAILURE;
    }
    printf("ser_bit_no: %02x\n", ser_bit_no);

    if (tx) {
        for (int i = 0; i < stringc; i++) {
            for (int k = 0; strings[i][k] != '\0'; k++) {
                if (ser_add_byte_to_transmitter_queue(strings[i][k]) != OK) {
                    printf("Error writing character: ser_write_char() inside %s\n", __func__);
                    return EXIT_FAILURE;
                }
            }
        }
        if (ser_add_byte_to_transmitter_queue('.') != OK) {
            printf("Error writing character: ser_write_char() inside %s\n", __func__);
            return EXIT_FAILURE;
        }
        if (ser_write_char('A') != OK) {
            printf("Error writing character: ser_write_char() inside %s\n", __func__);
            return EXIT_FAILURE;
        }
    }

    printf("Going inside driver receive loop\n");
    int r, ipc_status;
    message msg;
    extern int ser_return_value;
    extern uint8_t c;
    do {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with %d", r);
      }
      printf("Received interrupt driver_receive\n");
      if (is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
            if (msg.m_notify.interrupts & BIT(ser_bit_no)) {
                printf("Processing interrupts\n");
                ser_ih_fifo();
                if (ser_return_value != OK) {
                    printf("ser_ih_fifo() returned error inside %s\n", __func__);
                    continue;
                }
                if (! tx) {
                    ser_read_bytes_from_receiver_queue();
                }
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
