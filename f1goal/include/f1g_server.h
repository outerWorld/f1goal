
#ifndef __F1G_SERVER_H
#define __F1G_SERVER_H

typedef struct _worker_s {
	int						worker_id; //here is the thread or process id
	worker_func_p 			pf_worker;
	context_p 				p_ctx;
	context_clean_func_p 	p_ctx_clean_f;	
}worker_t, *worker_p;

typedef struct _access_s {
	access_func_p 			pf_access;
	context_p				p_ctx;
	context_clean_func_p 	p_ctx_clean_f;
	int						que_num;
	queue_p					p_ques;
}access_t, *access_p;

typedef int (*access_func_p)();
typedef int (*worker_func_p)(worker_context_p);
typedef struct _serv_object_s {
	int 			workers_num;
	worker_p		p_workers;
	access_p		p_accessor;
}serv_object_t, *serv_object_p;

int serv_load(char *fpath, server_conf_p p_conf);

serv_object_p serv_create(server_conf_p p_conf);

int serv_set_access_cb(serv_object_p p_obj, access_func_p p_acc);

int serv_set_work_cb(serv_object_p p_obj, worker_func_p p_worker);

int serv_run(serv_object_p p_obj);

int serv_destroy(serv_object_p p_obj);

#endif // __F1G_SERVER_H
