
#ifndef __F1G_SERVER_H
#define __F1G_SERVER_H

#include "f1g_basic_types.h"
#include "f1g_queue.h"
#include "f1g_server_conf.h"

struct worker_t;
typedef struct worker_t * worker_p;
typedef queue_object_p queue_p;

typedef i32_t (*access_func_p)();
typedef i32_t (*worker_func_p)(worker_p worker);
typedef i32_t (*context_clean_func_p)();

typedef struct context_s {
	i32_t i;
}context_t, *context_p;

struct worker_t {
	i32_t					worker_id; //here is the thread or process id
	worker_func_p 			pf_worker;
	context_p 				p_ctx;
	context_clean_func_p 	p_ctx_clean_f;	
};//worker_t, *worker_p;

typedef struct _access_s {
	access_func_p 			pf_access;
	context_p				p_ctx;
	context_clean_func_p 	p_ctx_clean_f;
	i32_t					que_num;
	queue_p					p_ques;
}access_t, *access_p;

typedef struct _serv_object_s {
	i32_t 			workers_num;
	worker_p		p_workers;
	access_p		p_accessor;
}serv_object_t, *serv_object_p;

i32_t serv_load(string_t fpath, server_conf_p p_conf);

serv_object_p serv_create(server_conf_p p_conf);

i32_t serv_set_access_cb(serv_object_p p_obj, access_func_p p_acc);

i32_t serv_set_work_cb(serv_object_p p_obj, worker_func_p p_worker);

i32_t serv_run(serv_object_p p_obj);

i32_t serv_destroy(serv_object_p p_obj);

#endif // __F1G_SERVER_H
