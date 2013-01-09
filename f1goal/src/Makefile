INC=-I../include
CC=gcc
ALL_OBJS=

include Makefile.am

ALL_OBJS += ${OBJS}

%.o:%.c
	${CC} -c $< ${INC}

.PHONY:all clean 

all:${ALL_OBJS}

clean:
	rm -f *.o
