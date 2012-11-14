CC=gcc
TESTS=test_http test_template test_lang_code
LIB=../lib/libf1goal.a
C_FLAG=-g

test_http:test_http.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_template:test_template.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

test_lang_code:test_lang_code.o ${LIB}
	${CC} ${C_FLAG} -o $@ $^

%.o: %.c
	${CC} ${C_FLAG} -c $< -I../include

.PHONY:clean tests

tests:${TESTS}

clean:
	rm -f *.o
	rm -f ${TESTS}
