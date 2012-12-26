vpath include tools src
CC=gcc
RelPath=rel_packet
ToolRelPath=toolrel_packet
CFlag=-g
InstPath=/usr/bin/
SrcPath=./src
BinPath=./bin
LibPath=./lib
DepPath=./dep
ObjPath=./objs
IncPath=-I./include -I./tools
ToolPath=./tools
F1goalLib_Objs=${ObjPath}/f1g_basic_funcs.o ${ObjPath}/f1g_string.o ${ObjPath}/f1g_log.o ${ObjPath}/f1g_cmd_line.o ${ObjPath}/f1g_data_op.o ${ObjPath}/f1g_eth_scrapy.o ${ObjPath}/f1g_bit_array.o ${ObjPath}/f1g_http.o ${ObjPath}/f1g_http_parser.o ${ObjPath}/f1g_template.o ${ObjPath}/f1g_data_set.o ${ObjPath}/f1g_net_forge.o ${ObjPath}/f1g_lang_code.o ${ObjPath}/f1g_smart_conf.o
F1goalLib_S=libf1goal.a
F1goalLib_D=libf1goal.so
TOOL_LIB=${LibPath}/${F1goalLib_S} ${DepPath}/lib/libpcap.a
TOOLS=pksender pksender_v1 pksender_v2 pkreceiver pkreceiver_v1 pkreceiver_v2 simple_server simple_client lang_conv

${BinPath}/pksender:${ObjPath}/pksender.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/pksender_v1:${ObjPath}/pksender_v1.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/pksender_v2:${ObjPath}/pksender_v2.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/pkreceiver:${ObjPath}/pkreceiver.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/pkreceiver_v1:${ObjPath}/pkreceiver_v1.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/pkreceiver_v2:${ObjPath}/pkreceiver_v2.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/simple_server:${ObjPath}/simple_server.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/simple_client:${ObjPath}/simple_client.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread
${BinPath}/lang_conv:${ObjPath}/lang_conv.o ${TOOL_LIB}
	${CC} -o $@ $^ -L/usr/lib -lpthread

${LibPath}/${F1goalLib_S}:${F1goalLib_Objs}
	ar rcs $@ $^

${LibPath}/${F1goalLib_D}:${F1goalLib_Objs}
	${CC} -shared -o $@ $^

${ObjPath}/pksender.o:${ToolPath}/pksender.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv pksender.o ${ObjPath}

${ObjPath}/lang_conv.o:${ToolPath}/lang_conv.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv lang_conv.o ${ObjPath}

${ObjPath}/pksender_v1.o:${ToolPath}/pksender_v1.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv pksender_v1.o ${ObjPath}

${ObjPath}/pksender_v2.o:${ToolPath}/pksender_v2.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv pksender_v2.o ${ObjPath}

${ObjPath}/pkreceiver.o:${ToolPath}/pkreceiver.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv pkreceiver.o ${ObjPath}

${ObjPath}/pkreceiver_v1.o:${ToolPath}/pkreceiver_v1.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv pkreceiver_v1.o ${ObjPath}

${ObjPath}/pkreceiver_v2.o:${ToolPath}/pkreceiver_v2.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv pkreceiver_v2.o ${ObjPath}

${ObjPath}/simple_server.o:${ToolPath}/simple_server.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv simple_server.o ${ObjPath}

${ObjPath}/simple_client.o:${ToolPath}/simple_client.c
	${CC} ${CFlag} -c $< -I./include -I${DepPath}/include -I./tools
	mv simple_client.o ${ObjPath}

${ObjPath}/f1g_basic_funcs.o:${SrcPath}/f1g_basic_funcs.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_basic_funcs.o ${ObjPath}

${ObjPath}/f1g_string.o:${SrcPath}/f1g_string.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_string.o ${ObjPath}

${ObjPath}/f1g_log.o:${SrcPath}/f1g_log.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_log.o ${ObjPath}

${ObjPath}/f1g_cmd_line.o:${SrcPath}/f1g_cmd_line.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_cmd_line.o ${ObjPath}

${ObjPath}/f1g_data_op.o:${SrcPath}/f1g_data_op.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_data_op.o ${ObjPath}

${ObjPath}/f1g_bit_array.o:${SrcPath}/f1g_bit_array.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_bit_array.o ${ObjPath}

${ObjPath}/f1g_http.o:${SrcPath}/f1g_http.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_http.o ${ObjPath}

${ObjPath}/f1g_http_parser.o:${SrcPath}/f1g_http_parser.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_http_parser.o ${ObjPath}


${ObjPath}/f1g_eth_scrapy.o:${SrcPath}/f1g_eth_scrapy.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_eth_scrapy.o ${ObjPath}

${ObjPath}/f1g_data_set.o:${SrcPath}/f1g_data_set.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_data_set.o ${ObjPath}

${ObjPath}/f1g_template.o:${SrcPath}/f1g_template.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_template.o ${ObjPath}

${ObjPath}/f1g_net_forge.o:${SrcPath}/f1g_net_forge.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_net_forge.o ${ObjPath}

${ObjPath}/f1g_lang_code.o:${SrcPath}/f1g_lang_code.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_lang_code.o ${ObjPath}

${ObjPath}/f1g_smart_conf.o:${SrcPath}/f1g_smart_conf.c
	${CC} ${CFlag} -c $< -I./include
	mv f1g_smart_conf.o ${ObjPath}

.PHONY:all clean_obj clean_all rel_all rel_tool

all:libs  pksender pksender_v1 pksender_v2 pkreceiver pkreceiver_v1 pkreceiver_v2 simple_client simple_server lang_conv

pksender:${BinPath}/pksender
pksender_v1:${BinPath}/pksender_v1
pksender_v2:${BinPath}/pksender_v2
pkreceiver:${BinPath}/pkreceiver
pkreceiver_v1:${BinPath}/pkreceiver_v1
pkreceiver_v2:${BinPath}/pkreceiver_v2
simple_client:${BinPath}/simple_client
simple_server:${BinPath}/simple_server
lang_conv:${BinPath}/lang_conv

#libs:${LibPath}/${F1goalLib_S} ${LibPath}/${F1goalLib_D}
libs:${LibPath}/${F1goalLib_S}

rel_all:
	if [ -d ${RelPath} ]; then rm -r ${RelPath}/*; else mkdir ${RelPath}; fi
	mkdir -p ${RelPath}/objs
	cp -R dep ${RelPath}
	cp -R src ${RelPath}
	cp -R lib ${RelPath}
	cp -R include ${RelPath}
	cp -R tools ${RelPath}
	cp -R bin ${RelPath}
	cp -R doc ${RelPath}
	cp makefile ${RelPath}
	cp tools/*.readme ${RelPath}/bin
	tar -zcvf ${RelPath}.tar.gz ${RelPath}
	rm -rf ${RelPath}
rel_tool:
	if [ -d ${ToolRelPath} ]; then rm -r ${ToolRelPath}/*; else mkdir ${ToolRelPath}; fi
	cp -R bin ${ToolRelPath}
	cp tools/*.readme ${ToolRelPath}/bin

clean_all:clean_obj
	cd ${BinPath}
	rm -f ${TOOLS}
	cd ..
	rm -f ${LibPath}/${F1goalLib_S}
	rm -f ${LibPath}/${F1goalLib_D}

clean_obj:
	rm -f ${ObjPath}/*.o
