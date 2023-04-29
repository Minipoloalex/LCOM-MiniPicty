# name of the program (Minix service)
PROG=proj

.PATH: ${.CURDIR}/utils

# source code files to be compiled
SRCS = proj.c rtc.c utils.c test6.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DLAB5

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIBLM} ${LIBXPM}
LDADD += -llcf -llm -lxpm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
