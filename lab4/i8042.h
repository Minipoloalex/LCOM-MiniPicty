#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_KEYBOARD 1
#define IRQ_MOUSE    12

#define BREAK_ESC   0x81       
#define BREAK_CODE  BIT(7)
#define TWO_BYTES   0xE0

#define FULL_OUT_BUF  BIT(0)
#define FULL_IN_BUF   BIT(1)
#define AUX           BIT(5)
#define TIMEOUT_ERR   BIT(6)
#define PARITY_ERR    BIT(7)

#define KBC_STATUS_REG 0x64
#define KBC_OUT_REG    0x60
#define KBC_CMD_REG    0x64
#define KBC_READ_CMD   0x20
#define KBC_WRITE_CMD  0x60
#define KBC_WRITE_B_TO_MOUSE 0xD4
#define MOUSE_DISABLE_DATA_REP 0xF5
#define ENABLE_INT  BIT(0)

#define WAIT_KBC    0
#define MAX_ATTEMPTS 5

#endif
