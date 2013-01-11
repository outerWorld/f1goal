/*
 * Author: huang chunping
 * Date  : 2013-01-06
 */
#ifndef __F1G_SERVER_H
#define __F1G_SERVER_H

#include "f1g_basic_types.h"
#include "f1g_buffer.h"
#include "f1g_accessor.h"
#include "f1g_queue.h"
#include "f1g_server_conf.h"

// default file path of server configuration
#define DEFAULT_SERV_CONF_FPATH "serv.conf"

typedef que_obj_p queue_p;
typedef pthread_t worker_id_t;
typedef pthread_t access_id_t;
typedef void * context_t;

typedef void * (*access_func_p)(void *arg);
// callback function of thread
typedef void * (*worker_func_p)(void *arg);
// callback function of data process
typedef i32_t (*worker_data_fproc_p)(context_t ctx, buffer_p p_rdbuf, buffer_p p_wrbuf, i32_t *proc_st);
typedef i32_t (*context_clean_func_p)(context_t ctx);

enum {
	PROC_ST_INIT = 0x00,
	PROC_ST_BRKOUT, 	// break out.
	PROC_ST_SENDBUF, 	// sent data in write buffer.
	PROC_ST_ERR,		// error occurs while processing data.
};

typedef struct worker_s {
	worker_id_t				id; //here is the thread or process id
	worker_func_p 			p_worker_f;
	worker_data_fproc_p		p_proc_f;
	context_t 				p_ctx;
	context_clean_func_p 	p_ctx_clean_f;	
	que_obj_p				p_que;
	buffer_t				rdbuf;
	buffer_t				wrbuf;
}worker_t, *worker_p;

typedef struct _access_s {
	access_id_t				id;
	accessor_t				accessor;
	access_func_p 			p_access_f;
	context_t				p_ctx;
	context_clean_func_p 	p_ctx_clean_f;
	i32_t					que_num;
	queue_p	*				p_ques;
}access_t, *access_p;

typedef struct _serv_object_s {
	i32_t 			worker_num;
	worker_p		p_workers;
	access_p		p_access;
}serv_object_t, *serv_object_p;

i32_t serv_load(string_t fpath, server_conf_p p_conf);

serv_object_p serv_create(server_conf_p p_conf);

i32_t serv_set_access_cb(serv_object_p p_obj, access_func_p p_acc_f,
						 context_t p_ctx, context_clean_func_p p_clean_f);

i32_t serv_set_work_cb(serv_object_p p_obj, worker_func_p p_worker,
						worker_data_fproc_p p_proc_f,
						context_t p_ctx, i32_t ctx_size,
						context_clean_func_p p_clean_f);

i32_t serv_run(serv_object_p p_obj);

i32_t serv_destroy(serv_object_p p_obj);

#endif // __F1G_SERVER_H
