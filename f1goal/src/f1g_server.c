
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "f1g_smart_conf.h"
#include "f1g_server.h"
#include "f1g_ipc_msg.h"

static void * access_cb_dft(void *arg);
static void * worker_cb_dft(void *arg);
static i32_t worker_data_fproc_dft(context_t ctx, buffer_p p_rdbuf,
								   buffer_p p_wrbuf, i32_t *proc_st);
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

	if (F1G_OK != smart_conf_get_i32(&cfg_eng, "sys", "que_size", 512, &p_conf->que_size)) {
		return F1G_ERR;
	}

	if (F1G_OK != smart_conf_get_i32(&cfg_eng, "sys", "que_cap", 1024, &p_conf->que_cap)) {
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

static i32_t worker_data_fproc_dft(context_t ctx, buffer_p p_rdbuf,
								   buffer_p p_wrbuf, i32_t *proc_st)
{
	i32_t len = 0;
	i8_p response = "\n\t so it's my response";

	*proc_st = PROC_ST_INIT;

	// here, do not call buffer_clear to p_wrbuf, as the caller may have put some data in write buffer.
	
	buffer_append(p_wrbuf, buffer_data(p_rdbuf), buffer_data_len(p_rdbuf));
	buffer_append(p_wrbuf, response, strlen(response));
	//buffer_show(p_wrbuf);

	// tell the caller to send buffer to client.
	*proc_st = PROC_ST_SENDBUF;

	return F1G_OK;
}

serv_object_p serv_create(server_conf_p p_conf)
{
	i32_t	i;
	que_obj_p p_que;
	serv_object_p p_obj;

	p_obj = (serv_object_p)malloc(sizeof(serv_object_t));
	if (!p_obj) return NULL;
	memset(p_obj, 0x00, sizeof(serv_object_t));

	p_obj->p_access = (access_p)malloc(sizeof(access_t));
	if (!p_obj->p_access) return NULL;
	memset(p_obj->p_access, 0x00, sizeof(access_t));

	p_obj->p_workers = (worker_p)malloc(sizeof(worker_t)*p_conf->worker_num);
	if (!p_obj->p_workers) return NULL;
	memset(p_obj->p_workers, 0x00, sizeof(worker_t)*p_conf->worker_num);
	p_obj->worker_num = p_conf->worker_num;
	
	// retrive queues for workers and accessor
	p_obj->p_access->p_ques = (que_obj_p*)malloc(sizeof(que_obj_p)*p_conf->worker_num);	
	if (!p_obj->p_access->p_ques) return NULL;
	p_obj->p_access->que_num = p_conf->worker_num;
	p_obj->p_access->p_access_f;

	for (i=0; i<p_obj->worker_num; i++) {
		p_que = que_obj_create(p_conf->que_size, p_conf->que_cap);
		if (!p_que) return NULL;
		que_obj_stat(p_que);
		p_obj->p_workers[i].p_que = p_que;
		p_obj->p_workers[i].p_proc_f = worker_data_fproc_dft;
		p_obj->p_workers[i].p_worker_f = worker_cb_dft;
		p_obj->p_access->p_ques[i] = p_que;
		buffer_init(&p_obj->p_workers[i].rdbuf, 2048);
		buffer_init(&p_obj->p_workers[i].wrbuf, 2048);
	}

	if (F1G_OK != accessor_init(&p_obj->p_access->accessor, LINKER_EPOLL, SOCK_TYPE_LTCP, p_conf->serv_win)) {
		fprintf(stderr, "accessor_init fail!\n");
		return NULL;
	}
	
	return p_obj;
}

i32_t serv_set_access_cb(serv_object_p p_obj, access_func_p p_acc_f,
						 context_t p_ctx, context_clean_func_p p_clean_f)
{

	return F1G_OK;
}

i32_t serv_set_work_cb(serv_object_p p_obj, worker_func_p p_worker,
						worker_data_fproc_p p_proc_f,
						context_t p_ctx, i32_t ctx_size,
						context_clean_func_p p_clean_f)
{
	i32_t i;
	context_t p = NULL;

	for (i=0; i<p_obj->worker_num; i++) {
		if (p_worker)
			p_obj->p_workers[i].p_worker_f = p_worker;
		if (p_proc_f)
			p_obj->p_workers[i].p_proc_f = p_proc_f;

		// store context for execute unit
		if (!p_ctx || ctx_size <= 0) continue;
		if (!(p = (context_t)malloc(ctx_size))) {
			return F1G_ERR;
		}
		p_obj->p_workers[i].p_ctx = p;
		p_obj->p_workers[i].p_ctx_clean_f = p_clean_f;
	}

	return F1G_OK;
}

static void *worker_cb_dft(void *arg)
{
	sock_info_p p_si;
	i8_p 	  	data = NULL;
	i32_t		r_flag = 1;
	i32_t		data_len = 0;
	i32_t		send_stat = 0;
	i32_t		recv_stat = 0;
	i32_t		proc_stat = PROC_ST_INIT;
	worker_p 	p_worker = (worker_p)arg;
	que_obj_p 	p_que = p_worker->p_que;
	buffer_p	p_rdbuf = &p_worker->rdbuf;
	buffer_p	p_wrbuf = &p_worker->wrbuf;
	
	while (r_flag) {
		if (que_obj_empty(p_que)) {
			usleep(5);
			continue;
		}

		// preprocess the data
		data = que_obj_head(p_que);
		data_len = BLK_DATA_LEN(data);
		p_si = (sock_info_p)BLK_DATA(data);
		//sock_info_show(p_si);
		if (SST_DATA_IN == p_si->sock_status) {
			buffer_clear(p_rdbuf);	
			nonblk_recvfrom(p_si->fd, p_rdbuf, &recv_stat);
			buffer_clear(p_wrbuf);
			if (recv_stat&RECV_OK && buffer_data_len(p_rdbuf) > 0) {
				// data process
				//buffer_show(p_rdbuf);
				if (0 != p_worker->p_proc_f(p_worker->p_ctx, p_rdbuf, p_wrbuf, &proc_stat)) {
					break;
				}
			}
			//fprintf(stdout, "recv_stat = %08x, %08x\n", recv_stat, recv_stat&RECV_DISC);
			if (recv_stat & RECV_DISC) {
				//fprintf(stdout, "it seems %d has disconnected", p_si->fd);
				close(p_si->fd);
			}
		}

		switch (proc_stat) {
			case PROC_ST_INIT:
				break;
			case PROC_ST_BRKOUT: 
				r_flag = 0;
				break;
			case PROC_ST_SENDBUF:
				nonblk_sendto(p_si->fd, p_si->src_ip, p_si->src_port, p_wrbuf, &send_stat);	
				break;
			case PROC_ST_ERR:
				break;
			default:
				break;
		}

		// postprocess the data

		que_obj_pop(p_que);
	}

	return NULL;
}

static u32_t dig_hash(i8_p key, i32_t key_size)
{
	u32_t val = 0;

	while (key_size-- > 0) {
		val = (val << 5) + *(key++);
	}

	return val;
}

static i32_t sel_que_by_ip(u32_t ip, i32_t que_num)
{
	return dig_hash((i8_p)&ip, sizeof(ip)) % que_num;
}

static void * access_cb_dft(void *arg)
{
	i32_t 		que_sel = 0; // select queue
	buffer_t 	buff = { 0 };
	que_obj_p 	p_sel_que = NULL;
	access_p 	p_acc = (access_p)arg;
	accessor_p	accessor = &p_acc->accessor;
	sock_info_t sock_info;
	
	while (1) {
		// detect there is any data
		if (0 == accessor_detect(accessor)) {
			continue;
		}

		// get data
		while (accessor_check_status(accessor, &sock_info)) {
			// select the queue for sending data
			que_sel = sel_que_by_ip(sock_info.src_ip, p_acc->que_num);
			//fprintf(stdout, "select queue [%d]\n", que_sel);
			p_sel_que = p_acc->p_ques[que_sel];
			
			// get buffer for storing received data.
			buff.size = BLK_SIZE(p_sel_que); // the size is total block size substracted the block header
			buff.buf = que_obj_next_freeblk(p_sel_que); // it includes the block header
			
			memcpy(BLK_DATA(buff.buf), &sock_info, sizeof(sock_info_t));
			buff.len = sizeof(sock_info_t);

			// receive data.
		
			BLK_DATA_LEN(buff.buf) = buff.len;
		
			// preprocess

			// send data to queue
			que_obj_move_next(p_sel_que);
			//que_obj_stat(p_sel_que);

			// post process
			//
		}

		accessor_clear(accessor);
	}

	return NULL;
}

i32_t serv_run(serv_object_p p_obj)
{
	i32_t i;
	worker_func_p p_wkf = worker_cb_dft;
	access_func_p p_acf = access_cb_dft;

	for (i=0; i<p_obj->worker_num; i++) {
		if (p_obj->p_workers[i].p_worker_f) p_wkf = p_obj->p_workers[i].p_worker_f;
		if (0 != pthread_create(&p_obj->p_workers[i].id, NULL, p_wkf, &p_obj->p_workers[i])) {
			return F1G_ERR;
		}
	}

	if (p_obj->p_access->p_access_f) p_acf = p_obj->p_access->p_access_f;
	if (0 != pthread_create(&p_obj->p_access->id, NULL, p_acf, p_obj->p_access)) {
		return F1G_ERR;
	}

	return F1G_OK;
}

i32_t serv_destroy(serv_object_p p_obj)
{
	i32_t i;

	for (i=0; i<p_obj->worker_num; i++) {
		if (p_obj->p_workers[i].p_ctx && p_obj->p_workers[i].p_ctx_clean_f) {
			p_obj->p_workers[i].p_ctx_clean_f(p_obj->p_workers[i].p_ctx);
			free(p_obj->p_workers[i].p_ctx);
			p_obj->p_workers[i].p_ctx = NULL;
		}
	}
	free(p_obj->p_workers);

	if (p_obj->p_access) {
		for (i=0; i<p_obj->p_access->que_num; i++) {
			que_obj_destroy(p_obj->p_access->p_ques[i]);
			free(p_obj->p_access->p_ques[i]);
			p_obj->p_access->p_ques[i] = NULL;
		}

		if (p_obj->p_access->p_ctx && p_obj->p_access->p_ctx_clean_f) {
			p_obj->p_access->p_ctx_clean_f(p_obj->p_access->p_ctx);
			free(p_obj->p_access->p_ctx);
			p_obj->p_access->p_ctx = NULL;
		}

		free(p_obj->p_access);
	}

	return F1G_OK;
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

	serv_run(p_server);

	serv_destroy();

	return 0;
}
#endif
