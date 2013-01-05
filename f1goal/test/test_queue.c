
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <pthread.h>

#include "f1g_queue.h"


static void * consumer_cb(void *argv)
{
}

// test_queue consumer_number
int main(int argc, char *argv[])
{
	int i = 0;
	int c_num = 0;
	pthread_t tid = 0;
	que_obj_p p_ques = NULL;

	// process input arguments
	if (argc < 2) {
		c_num = 1;
	} else {
		c_num = atoi(argv[1]);
		c_num = c_num % 255;
	}
	fprintf(stdout, "consumer number = %d\n", c_num);
	
	for (i=0; i<c_num; i++) {
	}

	return 0;
}
