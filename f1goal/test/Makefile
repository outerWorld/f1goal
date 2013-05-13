CC=gcc
TESTS=test_http test_template test_lang_code test_smart_conf test_server test_queue test_cli test_url_parser test_bitree test_basic_alg test_perf
LIB=../lib/libf1goal.a
C_FLAG=-g

test_perf:test_perf.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_http:test_http.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_template:test_template.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_lang_code:test_lang_code.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_smart_conf:test_smart_conf.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_server:test_server.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^ -lpthread

test_cli:test_cli.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_queue:test_queue.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^ -lpthread

test_url_parser:test_url_parser.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_bitree:test_bitree.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_basic_alg:test_basic_alg.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

%.o: %.c
	${CC} ${C_FLAG} -c $< -I../include

.PHONY:clean tests

tests:${TESTS}

clean:
	rm -f *.o
	rm -f ${TESTS}
