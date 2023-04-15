#include "test7.h"
#include "serial_port.h"

int (ser_test_conf)(unsigned short base_addr) {
    /* LCR and IER */
    uint8_t lcr, ier;
    if (util_sys_inb(base_addr + SER_LCR, &lcr)) {  // read line control register
        printf("Error reading LCR\n");
        return 1;
    };
    printf("LCR: 0x%02x\n", lcr);

    // LCR fields
    uint8_t n_bits_per_char;
    switch (lcr & SER_LCR_BITS_PER_CHAR) {  // switch case equivalent to (lcr & SER_LCR_BITS_PER_CHAR) + 5
        case SER_LCR_5BIT:
            n_bits_per_char = 5;
            break;
        case SER_LCR_6BIT:
            n_bits_per_char = 6;
            break;
        case SER_LCR_7BIT:
            n_bits_per_char = 7;
            break;
        case SER_LCR_8BIT:
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
    
    if (dlab == SER_LCR_DIVISOR_LATCH_ACCESS) {
        // set dlab to 0
        if (sys_outb(base_addr + SER_LCR, lcr & ~SER_LCR_DLAB) != OK) return EXIT_FAILURE;

        printf("Sent to LCR: 0x%02x\n", lcr & ~SER_LCR_DLAB);
    }

    if (util_sys_inb(base_addr + SER_IER, &ier)) {  // read interrupt enable register
        printf("Error reading IER\n");
        return 1;
    };
    
    printf("IER: 0x%02x\n", ier);
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
    
    // set dlab to 1
    if (sys_outb(base_addr + SER_LCR, lcr | SER_LCR_DLAB) != OK) return EXIT_FAILURE;
    printf("Sent to LCR: 0x%02x\n", lcr | SER_LCR_DLAB);

    uint8_t dll, dlm;
    if (util_sys_inb(base_addr + SER_DLL, &dll)) {  // read divisor latch LSB
        printf("Error reading DLL\n");
        return 1;
    };
    if (util_sys_inb(base_addr + SER_DLM, &dlm)) {  // read divisor latch MSB
        printf("Error reading DLM\n");
        return 1;
    };
    printf("DLL: 0x%02x\n", dll);
    printf("DLM: 0x%02x\n", dlm);
    uint16_t divisor = (dlm << 8) | dll;
    printf("Divisor: %d\n", divisor);
    uint32_t bitrate = SER_MAX_BITRATE / divisor;
    printf("Bitrate: %d\n", bitrate);

    // reset lcr (reset dlab to beginning value)
    if (sys_outb(base_addr + SER_LCR, lcr) != OK) return EXIT_FAILURE;
    
    
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
    // 
    return 0;
}

int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits, 
                    unsigned long stop, long parity, unsigned long rate, 
                    int stringc, char *strings[]) {
    
    return 0;
}

int ser_test_int(/* details to be provided */) { 
    /* To be completed */
    return 0;
}

int ser_test_fifo(/* details to be provided */) {
    /* To be completed */
    return 0;
}
