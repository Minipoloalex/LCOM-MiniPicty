#pragma once

#define SER_COM1 0x3F8  // IRQ 4, vector 0x0C
#define SER_COM1_IRQ 4

#define SER_COM2 0x2F8  // IRQ3, vector 0x0B
#define SER_COM2_IRQ 3

#define SER_MAX_BITRATE 115200

#define SER_DATA 0      // data register
#define SER_TX_RDY BITSER_COM1_IRQ(5)  // transmitter ready


#define SER_DLL 0x00    // divisor latch low (lsb)
#define SER_DLM 0x01    // divisor latch high (msb)

#define SER_RBR 0x00    // receiver buffer register (read)
#define SER_THR 0x00    // transmitter holding register (write)
#define SER_IER 0x01    // interrupt enable register (read/write)
#define SER_IIR 0x02    // interrupt identification register (read)
#define SER_FCR 0x02    // FIFO control register (write)
#define SER_LCR 0x03    // line control register (read/write)
#define SER_MCR 0x04    // modem control register (read/write)
#define SER_LSR 0x05    // line status register (read)
#define SER_MSR 0x06    // modem status register (read)
#define SER_SCR 0x07    // scratch register (read/write)


/* Dubious defines (start) -> Check if they are correct */
#define SER_IER_RDA BIT(0)  // enable receiver data available interrupt
#define SER_IER_THRE BIT(1)  // enable transmitter holding register empty interrupt
#define SER_IER_RLS BIT(2)  // enable receiver line status interrupt
#define SER_IER_MS BIT(3)   // enable modem status interrupt
/* Dubious defines (end) */


#define SER_LCR_BITS_PER_CHAR BIT(0) | BIT(1) // bits per character
#define SER_LCR_5_BITS_PER_CHAR 0x00   // 5 bits
#define SER_LCR_6_BITS_PER_CHAR BIT(0) // 6 bits
#define SER_LCR_7_BITS_PER_CHAR BIT(1) // 7 bits
#define SER_LCR_8_BITS_PER_CHAR 0x03   // 8 bits

#define SER_LCR_STOP_BIT BIT(2) // stop bit (0 for 1 stop bit, 1 for 2 stop bits)
#define SER_LCR_1_STOP_BIT 0x00
#define SER_LCR_2_STOP_BIT BIT(2)
/**
 * @brief 
 * X X 0 - no parity
 * 0 0 1 - odd parity
 * 0 1 1 - even parity
 * 1 0 1 - parity bit iss 1 (always)
 * 1 1 1 - parity bit is 0 (always)
 */
#define SER_LCR_PARITY_CTRL BIT(3) | BIT(4) | BIT(5)
/* CHECK IF PARITIES ARE CORRECT */
#define SER_LCR_8BIT_PAR BIT(3) // 8 bits, parity, 1 stop bit
#define SER_LCR_8BIT_2SB BIT(2) // 8 bits, no parity, 2 stop bits
#define SER_LCR_8BIT_PAR_2SB (BIT(3) | BIT(2)) // 8 bits, parity, 2 stop bits

#define SER_LCR_BREAK_CTRL BIT(6)

/**
 * @brief 
 * 1 - Divisor latch access
 * 0 - RBR (read) or THR (write)
 */
#define SER_LCR_DLAB BIT(7) // divisor latch access


#define SER_LCR_PARITY_NONE 0x00
#define SER_LCR_PARITY_ODD BIT(3)
#define SER_LCR_PARITY_EVEN (BIT(3) | BIT(4))
#define SER_LCR_PARITY_1 (BIT(3) | BIT(5))
#define SER_LCR_PARITY_0 (BIT(3) | BIT(4) | BIT(5))


/*
DLL and DLM access (DLAB = 1 in LCR)
To access DLL and DLM, a programmer must first set the DL Access bit (DLAB) in the LCR register.
Then, access DLL/DLM register and finally reset the DLAB bit, so other registers can be accessed.
*/

#define SER_LSR_DATA_READY BIT(0)   // data ready
#define SER_LSR_OE BIT(1)   // overrun error
#define SER_LSR_PE BIT(2)   // parity error
#define SER_LSR_FE BIT(3)   // framing error
// #define SER_LSR_BI BIT(4)   // break interrupt
#define SER_LSR_THRE BIT(5) // transmitter holding register empty
// #define SER_LSR_TEMT BIT(6) // transmitter empty
// #define SER_LSR_ERR BIT(7)  // error

/**
 * @brief IIR bits
 * 
 */
#define SER_IIR_INT_PEND 0x00
#define SER_IIR_INT_ID (BIT(1) | BIT(2) | BIT(3))
#define SER_IIR_INT_ID_POSITION 1

#define SER_IIR_INT_ID_NONE 0x01

// #define SER_IIR_INT_ID_MSI 0x00  // not necessary
#define SER_IIR_INT_ID_LS (BIT(0) | BIT(1))
#define SER_IIR_INT_ID_RDA BIT(1)
#define SER_IIR_INT_ID_CTI BIT(2)
#define SER_IIR_INT_ID_THRE BIT(0)

#define SER_IIR_FIFO_EN (BIT(6) | BIT(7))


#define QUEUE_SIZE 100
