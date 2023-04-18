# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/src/devices/controllers
.PATH: ${.CURDIR}/src/devices/controllers/kbc
.PATH: ${.CURDIR}/src/devices/keyboard
.PATH: ${.CURDIR}/src/devices/mouse
.PATH: ${.CURDIR}/src/devices/timer
.PATH: ${.CURDIR}/src/devices/video_card

.PATH: ${.CURDIR}/src/modules/interrupts

.PATH: ${.CURDIR}/src/utils/

SRCS = proj.c kbc.c keyboard.c mouse.c timer.c video_card.c utils.c interrupts.c

# additional compilation flags
CPPFLAGS += -pedantic -D__LCOM_OPTIMIZED__


# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf


# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>