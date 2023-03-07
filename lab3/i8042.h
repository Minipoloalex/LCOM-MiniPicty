// MACROS GOES HERE

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ 1


#define ESC_BREAK 0x81

#define KBC_STATUS_REG 0x64
#define KBC_CMD_REG 0x64
#define KBC_OUT_BUF 0x60
#define KBC_IN_REG 0x60

#define KBC_2BYTE_SCANCODE 0xE0

/* Keyboard-Related KBC Commands for PC-AT/PS2 */
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60
#define KBC_CHECK_KBC 0xAA
#define KBC_CHECK_INTERFACE 0xAB
#define KBC_ENABLE_INTERFACE 0xAD
#define KBC_DISABLE_INTERFACE 0xAE

#define KBC_PARITY_ERR BIT(7)
#define KBC_TIMEOUT_ERR BIT(6)

#define KBC_ENABLE_INT BIT(0)

#define BREAK_CODE BIT(7)

#endif /* _LCOM_I8042_H */
