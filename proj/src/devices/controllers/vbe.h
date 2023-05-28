#ifndef _VBE_H_
#define _VBE_H_

/** @defgroup uart UART
 * @{
 *
 * @brief Constants for programming the VBE Graphics Card.
 */

#define INVOKE_VBE_FUNCT 0x4F

#define BIOS_VIDEO_SERVICES 0x10

// BX register
#define LINEAR_FRAMEBUFFER BIT(14)
#define SET_START_OF_DISPLAY 0x80

// VBE functions
#define GET_VBE_CONTROLLER_INFO 0x00
#define GET_VBE_MODE_INFO 0x01
#define SET_VBE_MODE 0x02
#define VERTICAL_RETRACE 0x07


// Return values in AH
#define VBE_SUCCESS 0x00
#define VBE_FUNCT_FAILED 0x01
#define VBE_FUNCT_NOT_SUPPORTED 0x02
#define VBE_FUNCT_INVALID 0x03

// Graphics modes
#define GRAPHICS_MODE_0 0x105 // 1024x768 Indexed 8
#define GRAPHICS_MODE_1 0x110 // 640x480 Direct color 15((1:)5:5:5)
#define GRAPHICS_MODE_2 0x115 // 800x600 Direct color 24 (8:8:8)
#define GRAPHICS_MODE_3 0x11A // 1280x1024 Direct color 16 (5:6:5)
#define GRAPHICS_MODE_4 0x14C // 1024x768 Direct color 24 (8:8:8)

#endif
