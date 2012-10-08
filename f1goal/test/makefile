CC=gcc
TESTS=test_http
LIB=../lib/libf1goal.a

test_http:test_http.o ${LIB}
	${CC} -o $@ $^

%.o: %.c
	${CC} -c $< -I../include

.PHONY:clean tests

tests:${TESTS}

clean:
	rm -f *.o
	rm -f ${TESTS}
