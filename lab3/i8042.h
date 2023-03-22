#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define IRQ_KEYBOARD 1

#define BREAK_ESC   0x81       
#define BREAK_CODE  BIT(7)    // Break Code = Make Code | Bit(7)
#define TWO_BYTES   0xE0      // When Scancode has 2B, the Most Significant one is 0xE0

//    Status Byte
//    | 7         | 6            | 5 | 4 | 3 | 2 | 1              | 0 
//    | Par. err. | Timeout err. | x | x | x | x | In. Buff. Full | Out. Buff. Full
#define TIMEOUT_ERR   BIT(6)
#define PARITY_ERR    BIT(7)
#define FULL_OUT_BUF  BIT(0)
#define FULL_IN_BUF   BIT(1)

#define KBC_STATUS_REG 0x64     // Status is read from 0x64
#define KBC_OUT_REG    0x60     // Output (scancode) is read from 0x60
#define KBC_CMD_REG    0x64     // Input command: read KBC_cmd_Byte (0x20) or KBC_cmd_Byte (0x60)
#define KBC_READ_CMD   0x20
#define KBC_WRITE_CMD  0x60
//    KBC Command Byte
//    | 7 | 6 | 5             | 4             | 3 | 2 | 1                    | 0 
//    | x | x | Disable Mouse | Dis. Keyboard | x | x | Enable Mouse Interr. | En. Keyboard Interr.
#define ENABLE_INT  BIT(0)

#define WAIT_KBC    0
#define MAX_ATTEMPTS 5

#endif
