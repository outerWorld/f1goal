
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <pthread.h>

#include <errno.h>
#include <time.h>
#include <sys/time.h>

#include "f1g_queue.h"


#define LINE_SIZE 1024
static char line[LINE_SIZE+2] = { 0 };

static void * consumer_cb(void *arg)
{
	char *p = NULL;
	elem_p p_elem = NULL;
	que_obj_p p_que = (que_obj_p)arg;

	while (1) {
		if (que_obj_empty(p_que)) {
			usleep(1);
			continue;
		}
		p = que_obj_head(p_que);
		p_elem = (elem_p)p;
		fprintf(stdout, "queue[%p], len=%d, data=[%s]\n", p_que, p_elem->data_len, p+sizeof(elem_t));
		que_obj_pop(p_que);
	}

	return NULL;
}


// test_queue consumer_number
int main(int argc, char *argv[])
{
	int i = 0;
	int c_num = 0;
	pthread_t tid = 0;
	que_obj_p * p_ques = NULL;

	// process input arguments
	if (argc < 2) {
		c_num = 1;
	} else {
		c_num = atoi(argv[1]);
		c_num = c_num % 255;
	}
	fprintf(stdout, "consumer number = %d\n", c_num);

	p_ques = (que_obj_p*)malloc(sizeof(que_obj_p)*c_num);
	if (!p_ques) return -1;

	
	for (i=0; i<c_num; i++) {
		p_ques[i] = que_obj_create(1024, 512);
		if (!p_ques[i]) {
			fprintf(stderr, "que_obj_create fail!\n");
			return -1;
		}
		if (pthread_create(&tid, NULL, consumer_cb, p_ques[i]) != 0) {
			fprintf(stderr, "pthread_create fail [%d,%s]\n", errno, strerror(errno));
			return -1;
		}
	}

	i = 0;
	while (fgets(line, LINE_SIZE, stdin)) {
		line[strlen(line)-1] = '\0';
		if (!que_obj_full(p_ques[i])) {
			que_obj_add(p_ques[i], line, strlen(line)+1);
		} else {
			fprintf(stdout, "queue[%d] %p is full now\n", i, p_ques[i]);
		}
		if (++i >= c_num) i = 0;
	}

	fprintf(stdout, "test_queue exit!\n");

	return 0;
}
