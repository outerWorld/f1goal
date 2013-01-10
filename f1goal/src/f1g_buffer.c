#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	return F1G_OK;
}

i32_t buffer_set(buffer_p p_buf, i8_p buf, i32_t size)
{
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
