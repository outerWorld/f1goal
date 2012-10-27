#ifndef __LOG_H
#define __LOG_H

typedef struct _f1g_log_s {
    int     log_id;
    int     fd;
    int     buf_size;
    char *  buf; 
    int     data_len;
    int     w_sz_limit; // limit of write size 
}f1g_log_t, *f1g_log_p;

int f1g_log_init(f1g_log_p p_log, char *log_file, int buf_size, int limit);
int f1g_log_extern_buf(f1g_log_p p_log, int ext_buf_size);
int f1g_log_write(f1g_log_p p_log, char *fmt, ...); 
int f1g_log_destroy(f1g_log_p p_log);


#endif // __LOG_H
