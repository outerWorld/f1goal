INC=-I../include
CC=gcc
C_FLAGS=-g
ALL_OBJS=

include Makefile.am

ALL_OBJS += ${OBJS}

%.o:%.c
	${CC} -c $< ${INC} ${C_FLAGS}

.PHONY:all clean 

all:${ALL_OBJS}

clean:
	rm -f *.o
