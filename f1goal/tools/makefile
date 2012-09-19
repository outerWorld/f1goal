CC=gcc
INC=-I./ -I./include 

pksender:pksender_v1.o
	${CC} -o $@ $^
pkreceiver:pkreceiver_v1.o f1g_eth_scrapy.o ./lib/libpcap.a
	${CC} -o $@ $^ -L/usr/lib -lpthread
pksender_v1.o:pksender_v1.c
	${CC} -c $<
pkreceiver_v1.o:pkreceiver_v1.c
	${CC} -c $< ${INC} 
f1g_eth_scrapy.o:f1g_eth_scrapy.c
	${CC} -c $< ${INC} 

.PHONY:clean_obj clean_all all

all:pksender pkreceiver

clean_all: clean_obj
	rm -f pksender
	rm -f pkreceiver

clean_obj:
	rm -f *.o

