
#ifndef __F1G_NET_SERVER_H
#define __F1G_NET_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"
#include "f1g_socket_object.h"
#include "f1g_net_object.h"

#include "f1g_queue.h"

typedef i8_t (*server_cb_fun)();

typedef enum {
	EPOLL_MODEL = 0x00,
	POLL_MODEL,
	SELECT_MODEL,
	SERVER_MODEL_NUM
}NET_MODEL_E;

typedef enum {
	THREAD_MODEL = 0x00,
	PROCESS_MODEL,
	EXEC_MODEL_NUM
}EXEC_MODEL_E;

typedef enum {
	MUL_QUE_MODEL = 0x00,
	SINGLE_QUE_MODEL,
	QUE_MODEL_NUM
}QUE_MODEL_E;

typedef struct _net_server_s {
	net_object_p 	net_obj;
	i8_t 		 	que_num;
	queue_object_p 	msg_que;
}net_server_t, *net_server_p;

net_server_p net_server_create(SO_TYPE_E type, string_t ip, u16_t port);

i8_t net_server_load_config(string_t conf_file);

i8_t net_server_set_access_callbak(net_server_p p_obj, server_access_cb_fun fun);

i8_t net_server_set_worker_callbak(net_server_p p_obj, server_worker_cb_fun fun);

i8_t net_server_run(net_server_p p_obj);

i8_t net_server_destroy(net_server_p p_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_NET_SERVER_H
