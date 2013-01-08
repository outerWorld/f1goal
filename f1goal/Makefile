vpath include tools src
CC=gcc
RelPath=rel_packet
ToolRelPath=toolrel_packet
CFlag=-g -Werror
InstPath=/usr/bin/
SrcPath=./src
BinPath=./bin
LibPath=./lib
DepPath=./dep
ObjPath=./objs
IncPath=-I./include -I/usr/include
ToolPath=./tools

F1goalLib_S=libf1goal.a
F1goalLib_D=libf1goal.so
LIBF1GOAL=${F1goalLib_S} ${F1goalLib_D}
LIB_OBJS=

include src/Makefile.cc
LIB_OBJS += $(foreach obj,${OBJS}, src/${obj})

Donothing:
	@echo ${LIB_OBJS}

${F1goalLib_S}:${LIB_OBJS}
	echo $^
	ar rcs $@ $^
	mv $@ lib

${F1goalLib_D}:$(wildcard src/*.o)
	echo $^
	${CC} -shared -o $@ $^ -lpthread
	mv $@ lib

tests:
	cd test && sh build_tests.sh && cd ..

tools:
	cd tools && sh build_tools.sh && cd ..

gen_objs:
	cd src && make -f Makefile && cd ..

gen_libs:${LIBF1GOAL}

libs: gen_objs gen_libs

.PHONY:all clean_all

all:libs tests tools

clean_libs:
	cd lib && rm -f ${LIBF1GOAL} && cd ..

clean_tools:
	cd tools && make clean -f Makefile && cd ..

clean_objs:
	cd src && make clean -f Makefile && cd ..

clean_test:
	cd test && make clean -f Makefile && cd ..

clean_all: clean_libs clean_tools clean_objs clean_test

