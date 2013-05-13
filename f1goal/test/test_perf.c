
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "perf_mon.h"

int main(int argc, char *argv[])
{
	int i;
	time_t now;
	struct tm local = { 0 };

	time(&now);
	localtime_r(&now, &local);
	fprintf(stdout, "%d-%d-%d %d:%d:%d, %s\n", local.tm_year, local.tm_mon, local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec, asctime(&local));
	if (0 != mkdir("test0", 0777)) return -1;
	return 0;

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
