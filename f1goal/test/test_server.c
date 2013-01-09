
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_server.h"

static int test_ctx_clean_cb(void *test_ctx)
{
}

static int test_cb(void * test_ctx)
{
}

int main(int argc, char *argv[])
{
	server_conf_t serv_cfg;
	serv_object_p p_serv = NULL;
	
	if (argc < 2) return 1;

	if (F1G_OK != serv_load(argv[1], &serv_cfg)) {
		fprintf(stderr, "serv_load fail!\n");
		return 1;
	}
	p_serv = serv_create(&serv_cfg);
	if (!p_serv) return 1;
	
	serv_set_work_cb(p_serv, NULL, NULL, NULL, 0, NULL);

	serv_run(p_serv);
	
	while (1) {
		sleep(10);
	}

	serv_destroy(p_serv);
	free(p_serv);

	return 0;
}
