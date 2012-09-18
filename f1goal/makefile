vpath tools src
CC=gcc
InstPath=/usr/bin/
BinPath=./bin
LibPath=./lib
ObjPath=./objs
IncPath=-I./include
F1GOALLIB_OBJS=${ObjPath}/f1g_basic_funcs.o
F1GOALLIB_S=libf1goal.a
F1GOALLIB_D=libf1goal.so
TOOL_LIB=${LibPath}/${F1GOALLIB_S} ${LibPath}/libpcap.a -L/usr/lib -lpthread

${BinPath}/pksender:${ObjPath}/pksender.o ${TOOL_LIB}
	${CC} -o $@ $^ 
${ObjPath}/pksender.o:pksender.c
	${CC} -c $< ${INC}

${LibPath}/${F1GOALLIB_S}:${F1GOALLIB}
	ar rcs $@ $^

${LibPath}/${F1GOALLIB_D}:${F1GOALLIB}
	${CC} -shared $@ $^

.PHONY:all clean_obj clean_all

all:libs  pksender

libs:${LibPath}/${F1GOALLIB_S} ${LibPath}/${F1GOALLIB_D}

clean_all:clean_obj
	rm -f ${BinPath}/pksender
	rm -f ${LibPath}/${F1GOALLIB_S}
	rm -f ${LibPath}/${F1GOALLIB_D}

clean_obj:
	rm -f *.o
