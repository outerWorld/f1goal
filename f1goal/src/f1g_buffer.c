#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>

#include "f1g_buffer.h"

i32_t buffer_init(buffer_p p_buf, i32_t size)
{
	p_buf->buf = (i8_p)malloc(size);
	if (!p_buf->buf) return F1G_ERR;

	p_buf->size = size;
	p_buf->flag = 0x00;
	p_buf->len = 0;

	return F1G_OK;
}

i32_t buffer_enlarge(buffer_p p_buf, i32_t new_size)
{
	i8_p p_tmp = 0;

	if (new_size < p_buf->size) return F1G_OK;
	if (BUF_FLAG_REF == p_buf->flag) return F1G_ERR;

	if (new_size <= 0) new_size = 512;

	p_tmp = p_buf->buf;
	p_buf->buf = (i8_p)malloc(new_size);
	if (!p_buf->buf) {
		p_buf->buf = p_tmp;
		return F1G_ERR;
	}	
	p_buf->size = new_size;
	if (p_buf->len > 0) memcpy(p_buf->buf, p_tmp, p_buf->len);
	if (p_tmp) free(p_tmp);

	return F1G_OK;
}

/*
 * 
 */
i32_t buffer_set(buffer_p p_buf, i8_p buf, i32_t size)
{
	// Attention: here i do not have a good idea how to process
	// the flag with BUF_FLAG_ALLO, so let it return F1G_ERR first.
	// maybe some real applicant environments may help solve it.
	if (BUF_FLAG_ALLO == p_buf->flag) {
		return F1G_ERR;
	} else if (BUF_FLAG_REF == p_buf->flag) {
		p_buf->buf = buf;
		p_buf->size = size;	
	} else {
		return F1G_ERR;
	}

	return F1G_OK;
}

i32_t buffer_data_len(buffer_p p_buf)
{
	if (p_buf) return p_buf->len;

	return 0;
}

i8_p buffer_data(buffer_p p_buf)
{
	if (p_buf) return p_buf->buf;

	return NULL;
}

i32_t buffer_size(buffer_p p_buf)
{
	if (p_buf) return p_buf->size;

	return 0;
}

i32_t buffer_cap(buffer_p p_buf)
{
	return p_buf->size - p_buf->len;
}

i32_t buffer_append(buffer_p p_buf, i8_p p_data, i32_t data_len)
{
	i32_t len = 0;
	i32_t cap = 0;

	if (!p_buf || !p_data) return F1G_ERR;
	
	// for improving efficiency, buffer_cap may be altered by logic code: p_buf->size - p_buf->len
	cap = buffer_cap(p_buf);

	if (cap >= data_len) len = data_len;
	else len = cap;

	memcpy(p_buf->buf+p_buf->len, p_data, len);
	p_buf->len += len;
	p_buf->buf[p_buf->len] = '\0';

	return F1G_OK;
}

i32_t buffer_append_format(buffer_p p_buf, string_t fmt, ...)
{
	va_list vl;
	i32_t len = 0;
	i32_t cap = 0;
	
	cap = buffer_cap(p_buf);
	va_start(vl, fmt);
	len = vsnprintf(p_buf->buf + p_buf->len, cap, fmt, vl);
	if (len < 0) return F1G_ERR;
	else if (len > cap) {
		if (F1G_OK != buffer_enlarge(p_buf, p_buf->size + len + 128))
			return F1G_ERR;
		len = vsnprintf(p_buf->buf + p_buf->len, cap, fmt, vl);
	}

	p_buf->len += len;
	p_buf->buf[p_buf->len] = '\0';


	va_end(vl);
	return F1G_OK;
}

i32_t buffer_clear(buffer_p p_buf)
{
	if (p_buf && p_buf->buf) {
		p_buf->len = 0;
		p_buf->buf[0] = '\0';
	}

	return F1G_OK;
}

i32_t buffer_destroy(buffer_p p_buf)
{
	if (p_buf) {
		if (0x00 == p_buf->flag && p_buf->buf) free(p_buf->buf);
		memset(p_buf, 0x00, sizeof(buffer_t));
	}

	return F1G_OK;
}

i32_t buffer_show(buffer_p p_buf)
{
	fprintf(stdout, "buffer status: size=%d, len=%d, address=%p\n[%.*s]\n",
			p_buf->size, p_buf->len, p_buf->buf, p_buf->len, p_buf->buf);

	return F1G_OK;
}
