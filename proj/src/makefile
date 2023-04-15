# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c ser.c utils.c test7.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DLAB7

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF} ${LIBLM} ${LIBXPM}
LDADD += -llcf -llm -lxpm

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
