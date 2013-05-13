
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>

#include "perf_mon.h"

int main(int argc, char *argv[])
{
	int i;
	perf_mon_init(5, 10);

	perf_mon_add(0, "test0");
	perf_mon_add(1, "test1");

	while (1) {
		perf_mon_here(0, ST_START);
		usleep(100);
		perf_mon_here(0, ST_END);

		perf_mon_here(1, ST_START);
		usleep(200);
		perf_mon_here(1, ST_END);

		usleep(100);
	}

	perf_mon_destroy();

	return 0;
}
