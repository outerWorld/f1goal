CC=gcc
WORK_PATH=`pwd`
DepPath=../dep
LibPath=../lib
F1goalLib_S=libf1goal.a
TOOL_LIB=${LibPath}/${F1goalLib_S} ${DepPath}/lib/libpcap.a
TOOLS=pksender pksender_v1 pksender_v2 pkreceiver pkreceiver_v1 pkreceiver_v2 simple_server simple_client lang_conv

pksender:pksender.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
pksender_v1:pksender_v1.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
pksender_v2:pksender_v2.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
pkreceiver:pkreceiver.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
pkreceiver_v1:pkreceiver_v1.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
pkreceiver_v2:pkreceiver_v2.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
simple_server:simple_server.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
simple_client:simple_client.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
lang_conv:lang_conv.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread

.PHONY:all clean

all: ${TOOLS}

clean:
	rm -f *.o
	cd ../bin/ && rm -f ${TOOLS} && cd ${WORK_PATH}
