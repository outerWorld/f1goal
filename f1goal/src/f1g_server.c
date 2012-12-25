
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "f1g_general_conf.h"
#include "f1g_server.h"

int serv_load(char *fpath, server_conf_p p_conf)
{
}

serv_object_p serv_create(serer_conf_p p_conf)
{
	serv_object_p p_obj;

	p_obj = (serv_object_p)malloc(sizeof(serv_object_t));
	if (p_obj) {
		memset(p_obj, 0x00, sizeof(serv_object_t));
	}

	return p_obj;
}

int serv_set_access_cb(serv_object_p p_obj, access_func_p p_acc)
{
}

int serv_set_work_cb(serv_object_p p_obj, worker_func_p p_worker)
{
}

int serv_run(serv_object_p p_obj)
{
}

int serv_destroy(serv_object_p p_obj)
{
}

#ifdef __TEST_NET_SERVER

int main(int argc, char *argv[])
{
	char *serv_conf_fpath = NULL;
	server_conf_t conf;
	serv_object_p p_serv = NULL;

	if (0 != serv_load(serv_conf_fpath, &conf)) {
		return -1;
	}

	p_serv = serv_create(&conf);
	if (!p_serv) {
		return -1;
	}

	serv_set_access_cb(p_server, access_cb);
	serv_set_worker(p_server, worker_cb);

	serv_run(p_server);

	serv_stop();

	return 0;
}
#endif
