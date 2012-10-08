
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include "f1g_http.h"


typedef struct _http_header_info_s {
	u8_t id;
	u8_t header_size;
	i8_p header;
}http_header_info_t, *http_header_info_p;
http_header_info_t http_header_info[] = {
	{HTTP_HDR_METHOD, 0, ""},
	{HTTP_HDR_VERSION, 0, ""},
	{HTTP_HDR_PATH, 0, ""},
	{HTTP_HDR_HOST, 6, "Host: "},
	{HTTP_HDR_UA, 12, "User-Agent: "},
	{HTTP_HDR_COOKIE, 8, "Cookie: "},
	{HTTP_HDR_END, 0, ""},
	{HTTP_HDR_REQ_LINE, 0, ""},
	{HTTP_HDR_RSP_LINE, 0, ""},
	{0, 0, NULL}
};
static http_header_info_p _get_http_header_info(u8_t id)
{
	u8_t i;
	for (i=0; NULL!=http_header_info[i].header; i++) {
		if (id == http_header_info[i].id) {
			break;
		}
	}

	if (NULL == http_header_info[i].header) {
		return NULL;
	}
	
	return &http_header_info[i];
}

http_object_p http_object_create(i8_p obj_buf, u32_t buf_sz)
{
	u8_t	flag = 0;
	u32_t	ho_buf_sz;
	http_object_p p_obj;

	p_obj = (http_object_p)malloc(sizeof(http_object_t));
	if (NULL == p_obj) {
		return NULL;
	}
	memset(p_obj, 0x00, sizeof(http_object_t));

	if (NULL==obj_buf && buf_sz<HO_MIN_BUF_SZ) {
		ho_buf_sz = HO_DFT_BUF_SZ;
		flag = 1;
	} else if (NULL==obj_buf && buf_sz>HO_MIN_BUF_SZ) {
		ho_buf_sz = buf_sz;	
		flag = 1;
	} else if (NULL!=obj_buf && buf_sz>HO_MIN_BUF_SZ) {
		p_obj->obj_buf = obj_buf;
		p_obj->buf_sz = buf_sz;
	} else {
		free(p_obj);
		p_obj = NULL;
		return NULL;
	}

	if (1 == flag) {
		p_obj->obj_buf = (i8_p)malloc(ho_buf_sz);
		if (NULL == p_obj->obj_buf) {
			free(p_obj);
			p_obj = NULL;
			return NULL;
		}
		memset(p_obj->obj_buf, 0x00, ho_buf_sz);
		p_obj->buf_sz = ho_buf_sz;
	}
	
	if (F1G_OK != bit_array_init(&(p_obj->status), (u8_t)HTTP_HDR_NUM)) {
		free(p_obj->obj_buf);
		free(p_obj);
		return NULL;
	}
	
	return p_obj;
}

i8_t http_object_destroy(http_object_p p_obj)
{
	if (NULL != p_obj) {
		if (NULL != p_obj->obj_buf) {
			free(p_obj->obj_buf);
		}
		bit_array_clean(&(p_obj->status));
		free(p_obj);
	}

	return F1G_OK;
}

i8_t http_object_reset_buffer(http_object_p p_obj, i8_p obj_buf, u32_t buf_sz)
{
	if (NULL!=obj_buf && buf_sz>0) {
		free(p_obj->obj_buf);
		p_obj->obj_buf = obj_buf;
		p_obj->buf_sz = buf_sz;
	} else if (NULL==obj_buf && buf_sz>0) {
		free(p_obj->obj_buf);
		p_obj->obj_buf = (i8_p)malloc(buf_sz);
		if (NULL == p_obj->obj_buf) {
			return F1G_ERR;
		}
		p_obj->buf_sz = buf_sz;
	}
	
	return F1G_OK;
}

i8_t http_object_reset(http_object_p p_obj)
{
	memset(p_obj->obj_buf, 0x00, p_obj->buf_sz);
	p_obj->data_len = 0;
	memset(p_obj->hdr_set, 0x00, HTTP_HDR_NUM*sizeof(i8_p));
	memset(p_obj->hdr_len_set, 0x00, HTTP_HDR_NUM*sizeof(i16_t));
	p_obj->content = NULL;
	p_obj->content_len = 0;
	bit_array_reset(&(p_obj->status));

	return F1G_OK;
}


i8_t http_object_add_header(http_object_p p_obj, u8_t header_id, i8_p header, u16_t header_len)
{
	http_header_info_p p_hi = NULL;

	if (header_id >= HTTP_HDR_NUM) {
		return F1G_ERR;
	}

	// check existence of header
	if (1 == bit_array_bit_value(&p_obj->status, header_id) ) {
		return F1G_ERR;
	}

	p_hi = _get_http_header_info(header_id);
	if (NULL == p_hi) {
		return F1G_ERR;
	}

	// 2 means sizeof("\r\n")
	if ((p_obj->data_len+header_len+2+p_hi->header_size) >= p_obj->buf_sz) {
		return F1G_ERR;
	}

	if (p_hi->header_size > 0) {
		memcpy(p_obj->obj_buf+p_obj->data_len, p_hi->header, p_hi->header_size);
		p_obj->data_len += p_hi->header_size;
	}
	
	memcpy(p_obj->obj_buf+p_obj->data_len, header, header_len);
	p_obj->hdr_set[header_id] = p_obj->obj_buf + p_obj->data_len;
	p_obj->hdr_len_set[header_id] = header_len;
	p_obj->data_len += header_len;

	memcpy(p_obj->obj_buf+p_obj->data_len, "\r\n", 2);
	p_obj->data_len += 2;
	
	bit_array_set_bit(&p_obj->status, header_id);

	return F1G_OK;
}


i8_t http_object_add_content(http_object_p p_obj, i8_p content, u16_t content_len)
{
	if ((p_obj->data_len+content_len) > p_obj->buf_sz) {
		return F1G_ERR;
	}
	
	if (p_obj->content_len == 0) {
		p_obj->content = p_obj->obj_buf + p_obj->data_len;
	}
	
	memcpy(p_obj->content+p_obj->content_len, content, content_len);
	p_obj->content_len += content_len;	
	p_obj->data_len += content_len;

	return F1G_OK;
}

void http_object_print(http_object_p p_obj)
{
	if (p_obj->data_len) {
		printf("%.*s", p_obj->data_len, p_obj->obj_buf);
	}
}

i8_t http_object_get_headers(http_object_p p_obj, i8_p buf, u32_p buf_sz)
{
	
	return F1G_OK;
}

i8_t http_object_get_header(http_object_p p_obj, HTTP_HDR_E header_id, i8_p buf, u32_p buf_size) 
{
	return F1G_OK;
}

i8_t http_object_parse_data(http_object_p p_obj, string_t data, u16_t data_len)
{
	i8_p pb;
	i8_p pe;

	if (p_obj->buf_sz < data_len) {
		http_object_reset_buf(p_obj, NULL, data_len);
	}

	pb = data;
	pe = strstr(pb, "\r\n");

	while (pb != pe) {
	}

	return F1G_OK;
}

i8_t http_object_parse_file(http_object_p p_obj, string_t file)
{
	i32_t fd;
	ssize_t len;
	struct stat st;

	if (-1 == (fd=open(file, O_RDONLY, 0))) {
		return F1G_ERR;
	}
	
	if (-1 ==fstat(fd, &st)) {
		close(fd);
		return F1G_ERR;
	}
	
	if (st.st_size <= 0) {
		close(fd);
		return F1G_ERR;
	}

	if (p_obj->buf_sz < st.st_size) {
		if (F1G_ERR == http_object_reset_buf(NULL, st.st_size)) {
			close(fd);
			return F1G_ERR;
		}
	}
	
	if (len=read(fd, p_obj->obj_buf, st.st_size) != st.st_size) {
		close(fd);
		return F1G_ERR;
	}
	p_obj->data_len = len;

	if (F1G_OK != http_object_parse_data(p_obj, p_obj->obj_buf, len)) {
		//return F1G_ERR;
	}
	
	close(fd);

	return F1G_OK;
}



#ifdef _TEST_HTTP
int main(int argc, char *argv[])
{
	return 0;
}
#endif // _TEST_HTTP
