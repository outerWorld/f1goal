CC=gcc
TESTS=test_http test_template test_lang_code test_smart_conf test_server test_queue
LIB=../lib/libf1goal.a
C_FLAG=-g

test_http:test_http.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_template:test_template.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_lang_code:test_lang_code.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^
test_smart_conf:test_smart_conf.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^
test_server:test_server.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_queue:test_queue.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^ -lpthread

%.o: %.c
	${CC} ${C_FLAG} -c $< -I../include

.PHONY:clean tests

tests:${TESTS}

clean:
	rm -f *.o
	rm -f ${TESTS}
