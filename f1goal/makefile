vpath include tools src
CC=gcc
InstPath=/usr/bin/
SrcPath=./src
BinPath=./bin
LibPath=./lib
DepPath=./dep
ObjPath=./objs
IncPath=-I./include -I./tools
ToolPath=./tools
F1goalLib_Objs=${ObjPath}/f1g_basic_funcs.o ${ObjPath}/f1g_cmd_line.o ${ObjPath}/f1g_eth_scrapy.o
F1goalLib_S=libf1goal.a
F1goalLib_D=libf1goal.so
TOOL_LIB=${LibPath}/${F1goalLib_S} ${DepPath}/lib/libpcap.a

${BinPath}/pksender:${ObjPath}/pksender.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/pksender_v1:${ObjPath}/pksender_v1.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/pkreceiver:${ObjPath}/pkreceiver.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/pkreceiver_v1:${ObjPath}/pkreceiver_v1.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread

${LibPath}/${F1goalLib_S}:${F1goalLib_Objs}
	ar rcs $@ $^

${LibPath}/${F1goalLib_D}:${F1goalLib_Objs}
	${CC} -shared -o $@ $^

${ObjPath}/pksender.o:${ToolPath}/pksender.c
	${CC} -c $< -I./include -I${DepPath}/include -I./tools
	mv pksender.o ${ObjPath}

${ObjPath}/pksender_v1.o:${ToolPath}/pksender_v1.c
	${CC} -c $< -I./include -I${DepPath}/include -I./tools
	mv pksender_v1.o ${ObjPath}

${ObjPath}/pkreceiver.o:${ToolPath}/pkreceiver.c
	${CC} -c $< -I./include -I${DepPath}/include -I./tools
	mv pkreceiver.o ${ObjPath}

${ObjPath}/pkreceiver_v1.o:${ToolPath}/pkreceiver_v1.c
	${CC} -c $< -I./include -I${DepPath}/include -I./tools
	mv pkreceiver_v1.o ${ObjPath}

${ObjPath}/f1g_basic_funcs.o:${SrcPath}/f1g_basic_funcs.c
	${CC} -c $< -I./include
	mv f1g_basic_funcs.o ${ObjPath}

${ObjPath}/f1g_cmd_line.o:${SrcPath}/f1g_cmd_line.c
	${CC} -c $< -I./include
	mv f1g_cmd_line.o ${ObjPath}

${ObjPath}/f1g_eth_scrapy.o:${SrcPath}/f1g_eth_scrapy.c
	${CC} -c $< -I./include
	mv f1g_eth_scrapy.o ${ObjPath}


.PHONY:all clean_obj clean_all

all:libs  pksender pksender_v1 pkreceiver pkreceiver_v1

pksender:${BinPath}/pksender
pksender_v1:${BinPath}/pksender_v1
pkreceiver:${BinPath}/pkreceiver
pkreceiver_v1:${BinPath}/pkreceiver_v1

libs:${LibPath}/${F1goalLib_S} ${LibPath}/${F1goalLib_D}

clean_all:clean_obj
	rm -f ${BinPath}/pksender
	rm -f ${LibPath}/${F1goalLib_S}
	rm -f ${LibPath}/${F1goalLib_D}

clean_obj:
	rm -f ${ObjPath}/*.o
