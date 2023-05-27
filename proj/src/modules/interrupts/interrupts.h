#include <lcom/lcf.h>
#include <lcom/proj.h>

#include "../../devices/controllers/kbc/kbc.h"
#include "../../devices/keyboard/keyboard.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/video_card/video_card.h"
#include "../../devices/timer/timer.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../devices/rtc/rtc.h"

int(subscribe_interrupts)();
int(unsubscribe_interrupts)();
