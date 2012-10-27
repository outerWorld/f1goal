
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#include "f1g_log.h"

int f1g_log_init(f1g_log_p p_log, char *log_file, int buf_size, int limit)
{
	memset(p_log, 0x00, sizeof(f1g_log_t));
	p_log->buf_size = buf_size;
	p_log->buf = (char*)malloc(buf_size);
	if (NULL == p_log->buf) {
		fprintf(stderr, "%s,%d\n", __FUNCTION__, __LINE__);
		return -1;
	}
    memset(p_log->buf, 0x00, buf_size);
	p_log->w_sz_limit = limit;
    p_log->data_len = 0;

	p_log->fd = open(log_file, O_WRONLY|O_CREAT|O_APPEND, S_IWUSR|S_IRWXU);
	if (p_log->fd < 0) {
		fprintf(stderr, "%s,%d [Error=%d:%s]\n", __FUNCTION__, __LINE__, errno, strerror(errno));
		free(p_log->buf);
		return -1;
	}

	return 0;
}

int f1g_log_extern_buf(f1g_log_p p_log, int ext_buf_size)
{
    char *tmp = NULL;

    tmp = p_log->buf;
    p_log->buf = (char *)malloc(ext_buf_size);
    if (NULL == p_log->buf) {
        p_log->buf = tmp;
        return -1;
    }
    memset(p_log->buf, 0x00, ext_buf_size);

    if (p_log->data_len > 0) {
        memcpy(p_log->buf, tmp, p_log->data_len);
    }
    free(tmp);
    p_log->buf_size = ext_buf_size;
    p_log->w_sz_limit = ext_buf_size - 1024;

    return 0;
}

int f1g_log_write(f1g_log_p p_log, char *fmt, ...) 
{
	va_list argp;
	int len = 0;
    int cap = 0;

    if (strlen(fmt) == 0) {
		write(p_log->fd, p_log->buf, p_log->data_len);
		p_log->data_len = 0;
        return 0;
    }

    cap = p_log->w_sz_limit - p_log->data_len;
	va_start(argp, fmt);
	len = vsnprintf(p_log->buf+p_log->data_len, cap, fmt, argp);
	va_end(argp);
    if (len > cap) {
        //fprintf(stdout, "Log buffer len[%d] limit[%d], write len[%d]\n", p_log->data_len, p_log->w_sz_limit, len);
		write(p_log->fd, p_log->buf, p_log->data_len);
		p_log->data_len = 0;
        cap = p_log->buf_size - p_log->data_len;
        if (len > cap) {
            f1g_log_extern_buf(p_log, len+1024);
        }
	    va_start(argp, fmt);
        len = vsnprintf(p_log->buf+p_log->data_len, cap, fmt, argp);
	    va_end(argp);
    }
	p_log->data_len += len;

	if (p_log->data_len >= p_log->w_sz_limit) {
		write(p_log->fd, p_log->buf, p_log->data_len);
		p_log->data_len = 0;
	}

	return 0;
}

int f1g_log_destroy(f1g_log_p p_log)
{
    if (p_log->data_len > 0) {
        write(p_log->fd, p_log->buf, p_log->data_len);
    }

	close(p_log->fd);

	if (p_log->buf) {
		free(p_log->buf);
	}

	return 0;
}


