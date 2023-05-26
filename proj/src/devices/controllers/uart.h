#pragma once

#define SER_COM1 0x3F8  // IRQ 4, vector 0x0C
#define SER_COM1_IRQ 4

#define SER_COM2 0x2F8  // IRQ3, vector 0x0B
#define SER_COM2_IRQ 3

#define SER_MAX_BITRATE 115200


#define SER_DLL 0x00    // divisor latch low (lsb)
#define SER_DLM 0x01    // divisor latch high (msb)

#define SER_RBR 0x00    // receiver buffer register (read)
#define SER_THR 0x00    // transmitter holding register (write)
#define SER_IER 0x01    // interrupt enable register (read/write)
#define SER_IIR 0x02    // interrupt identification register (read)
#define SER_FCR 0x02    // FIFO control register (write)
#define SER_LCR 0x03    // line control register (read/write)
#define SER_LSR 0x05    // line status register (read)


#define SER_IER_RDA BIT(0)  // enable receiver data available interrupt
#define SER_IER_THRE BIT(1)  // enable transmitter holding register empty interrupt
#define SER_IER_RLS BIT(2)  // enable receiver line status interrupt
#define SER_IER_MS BIT(3)   // enable modem status interrupt


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
#define SER_LCR_BREAK_CTRL BIT(6) // not needed
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
#define SER_LSR_THRE BIT(5) // transmitter holding register empty

/**
 * @brief IIR bits
 * 
 */
/**
 * @brief Interrupt not pending
 * 
 */
#define SER_IIR_INT_NOT_PEND BIT(0)
#define SER_IIR_INT_ID (BIT(1) | BIT(2) | BIT(3))
#define SER_IIR_INT_ID_POSITION 1
#define SER_IIR_INT_ID_LS (BIT(0) | BIT(1))
#define SER_IIR_INT_ID_RDA BIT(1)
#define SER_IIR_INT_ID_CTI BIT(2) | BIT(1)
#define SER_IIR_INT_ID_THRE BIT(0)
#define SER_IIR_FIFO_EN (BIT(6) | BIT(7))


#define SER_FCR_ENABLE BIT(0)
#define SER_FCR_CLEAR_RECEIVER BIT(1)
#define SER_FCR_CLEAR_TRANSMITTER BIT(2)
#define SER_FCR_TRIGGER_LEVEL_4 (BIT(6))
#define SER_FCR_DEFAULT (SER_FCR_ENABLE | SER_FCR_CLEAR_RECEIVER | SER_FCR_CLEAR_TRANSMITTER | SER_FCR_TRIGGER_LEVEL_4)



/**
 * @brief Communication protocol
 * 
 */

#define SER_MOUSE_DRAWING           '.'
#define SER_MOUSE_NOT_DRAWING       '-'
#define SER_END                     0xFF
#define SER_END_REPLACEMENT         0xFE
#define SER_WORD_INDEX              '?'
#define SER_FIRST_BUTTON            'A'   // uppercase letters are used for buttons
// #define SER_TRASH                 '_'
