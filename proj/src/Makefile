# name of the program (Minix service)
PROG=proj

# Devices
.PATH: ${.CURDIR}
.PATH: ${.CURDIR}/devices/controllers
.PATH: ${.CURDIR}/devices/controllers/kbc
.PATH: ${.CURDIR}/devices/keyboard
.PATH: ${.CURDIR}/devices/mouse
.PATH: ${.CURDIR}/devices/timer
.PATH: ${.CURDIR}/devices/video_card
.PATH: ${.CURDIR}/devices/serial_port

.PATH: ${.CURDIR}/model/brush
.PATH: ${.CURDIR}/model/player
.PATH: ${.CURDIR}/model/position
.PATH: ${.CURDIR}/model/queue
.PATH: ${.CURDIR}/model

# Modules
.PATH: ${.CURDIR}/modules/interrupts
.PATH: ${.CURDIR}/modules/menu
.PATH: ${.CURDIR}/modules/game

.PATH: ${.CURDIR}/utils/

SRCS = proj.c kbc.c keyboard.c mouse.c timer.c video_card.c utils.c interrupts.c menu.c game.c serial_port.c queue.c position.c brush.c player.c

# additional compilation flags
CPPFLAGS += -pedantic -D__LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>