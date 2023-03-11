# name of the program (Minix service)
PROG=lab3 

# source code files to be compiled
<<<<<<< HEAD
SRCS = lab3.c keyboard.c ../lab2/utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CPPFLAGS += -D LAB3
=======
SRCS = lab3.c keyboard.c kbc.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic -DLAB3
>>>>>>> francisco_ana

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
