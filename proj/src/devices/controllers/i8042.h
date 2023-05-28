#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @defgroup i8042 i8042
 * @{
 *
 * @brief Constants for programming the i8042 KBC (Keyboard and Mouse).
 */

#ifndef BIT
#define BIT(n) (1 << (n))
#endif

#define IRQ_KEYBOARD 1
#define IRQ_MOUSE    12

#define MAKE_ENTER 0x1c
#define MAKE_BACKSPACE 0x0e
#define MAKE_SPACE 0x39
#define BREAK_ESC   0x81       
#define BREAK_CODE  BIT(7)
#define TWO_BYTES   0xE0

#define FULL_OUT_BUF  BIT(0)
#define FULL_IN_BUF   BIT(1)
#define AUX           BIT(5)
#define TIMEOUT_ERR   BIT(6)
#define PARITY_ERR    BIT(7)

//    KBC Command Byte
//    | 7 | 6 | 5             | 4             | 3 | 2 | 1                    | 0 
//    | x | x | Disable Mouse | Dis. Keyboard | x | x | Enable Mouse Interr. | En. Keyboard Interr.
#define KBC_STATUS_REG 0x64
#define KBC_OUT_REG    0x60
#define KBC_IN_REG    0x64
#define KBC_READ_CMD_BYTE   0x20
#define KBC_WRITE_CMD_BYTE  0x60
#define KBC_WRITE_MOUSE_CMD    0xD4

#define MOUSE_ENABLE_DATA_REP   0xF4
#define MOUSE_DISABLE_DATA_REP  0xF5
#define KEYBOARD_ENABLE_INT     BIT(0)

#define LB(packet_byte) (packet_byte & BIT(0))
#define RB(packet_byte) (packet_byte & BIT(1)) >> 1
#define MB(packet_byte) (packet_byte & BIT(2)) >> 2
#define DELTA_X(packet_byte)  ((packet_byte) & BIT(4))
#define DELTA_Y(packet_byte)  ((packet_byte) & BIT(5))
#define X_OV(packet_byte)     ((packet_byte) & BIT(6)) >> 6
#define Y_OV(packet_byte)     ((packet_byte) & BIT(7)) >> 7

#define FIRST_BYTE_VALIDATION BIT(3)

#define MOUSE_ACK   0xFA
#define MOUSE_NACK  0xFE
#define MOUSE_ERR   0xFC

#define WAIT_KBC    20000
#define MAX_ATTEMPTS 10

#endif
