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
	fprintf(stdout, "buffer status: size=%d, len=%d, address=%p\n",
			p_buf->size, p_buf->len, p_buf->buf);

	return F1G_OK;
}
