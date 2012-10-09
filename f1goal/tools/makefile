CC=gcc
INC=-I./ -I./include -I../include
LIB=../lib/libf1goal.a
pksender:pksender_v1.o
	${CC} -o $@ $^
pksender_v2:pksender_v2.o ${LIB}
	${CC} -g -o $@ $^
pkreceiver:pkreceiver_v1.o f1g_eth_scrapy.o ./lib/libpcap.a
	${CC} -o $@ $^ -L/usr/lib -lpthread
pksender_v1.o:pksender_v1.c
	${CC} -c $<
pksender_v2.o:pksender_v2.c
	${CC} -g -c $< ${INC} 
pkreceiver_v1.o:pkreceiver_v1.c
	${CC} -c $< ${INC} 
f1g_eth_scrapy.o:f1g_eth_scrapy.c
	${CC} -c $< ${INC} 

.PHONY:clean_obj clean_all all

all:pksender pkreceiver pksender_v2

clean_all: clean_obj
	rm -f pksender
	rm -f pksender_v2
	rm -f pkreceiver

clean_obj:
	rm -f *.o

