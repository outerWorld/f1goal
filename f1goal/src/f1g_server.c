
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "f1g_smart_conf.h"
#include "f1g_server.h"

// 
#define SERV_SYS_CFG "serv.ini"

i32_t serv_load(char *fpath, server_conf_p p_conf)
{
	smart_conf_t cfg_eng;

	memset(p_conf, 0x00, sizeof(server_conf_t));
	if (F1G_OK != smart_conf_init(&cfg_eng, 4, 20)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_read(&cfg_eng, fpath, CFT_INI)) {
		smart_conf_destroy(&cfg_eng);
		return F1G_ERR;
	}
	
	smart_conf_dbg(&cfg_eng);
	
	if (F1G_OK != smart_conf_get_i32(&cfg_eng, "sys", "work_mode", WK_MODE_MT, &p_conf->work_mode)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_get_i32(&cfg_eng, "sys", "worker_num", 1, &p_conf->worker_num)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_get_i32(&cfg_eng, "sys", "que_mode", 1, &p_conf->que_mode)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_get_i32(&cfg_eng, "sys", "que_size", 1, &p_conf->que_size)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_get_x32(&cfg_eng, "sys", "serv_type", SERV_TYPE_STCP, &p_conf->serv_type)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_get_str(&cfg_eng, "sys", "serv_win", "", p_conf->serv_win, SERV_WIN_DESC_SIZE)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_get_i32(&cfg_eng, "sys", "access_mode", ACCESS_SELECT, &p_conf->serv_type)) {
		return F1G_ERR;
	}

	smart_conf_destroy(&cfg_eng);

	return F1G_OK;
}

serv_object_p serv_create(server_conf_p p_conf)
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
