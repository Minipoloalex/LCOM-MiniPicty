#include <lcom/lcf.h>
#include <lcom/proj.h>

#include "../../devices/controllers/kbc/kbc.h"
#include "../../devices/keyboard/keyboard.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/video_card/video_card.h"
#include "../../devices/timer/timer.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../devices/rtc/rtc.h"

/** @defgroup interrupts Interrupts
 * @{
 *
 * @brief Module responsible for (un)subscribing the devices interrupts
 */

/**
 * @brief Subscribe all the devices interrupts used in the project 
 * 
 * @return int 0 if success, 1 otherwise
 */
int(subscribe_interrupts)();

/**
 * @brief Unsubscribe all the devices interrupts used in the project 
 * 
 * @return int 0 if success, 1 otherwise
 */
int(unsubscribe_interrupts)();
