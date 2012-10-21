
#ifndef __F1G_HTTP_H
#define __F1G_HTTP_H


#ifdef __cplusplus
#define extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"
#include "f1g_bit_array.h"

typedef enum {
	HTTP_HDR_METHOD = 0x00,
	HTTP_HDR_PATH,
	HTTP_HDR_VERSION,
	HTTP_HDR_HOST,
	HTTP_HDR_UA,
	HTTP_HDR_REFER,
	HTTP_HDR_COOKIE,
	HTTP_HDR_END,
	HTTP_HDR_REQ_LINE,
	HTTP_HDR_RSP_LINE,
	HTTP_HDR_NUM,
}HTTP_HDR_E;

// HTTP OBJECT DEFAULT BUFFER SIZE
#define HO_DFT_BUF_SZ 2048
#define HO_MIN_BUF_SZ 64

typedef struct _http_object_s {
	i8_p 		obj_buf;
	u32_t 		buf_sz;
	u32_t		data_len;
	bit_array_t status;	// stores information which header is set
	i8_p		hdr_set[HTTP_HDR_NUM];
	i16_t   	hdr_len_set[HTTP_HDR_NUM];
	i8_p 		content;
	u32_t		content_len;
}http_object_t, *http_object_p;

/*
 * specially, When obj_buf is NULL, and buf_sz is lower or equal to 0, 
 * then HO_DEF_BUF_SZ becomes effective, and a new object is created.
 */
http_object_p http_object_create(i8_p obj_buf, u32_t buf_sz);

/*
 * here, the obj_buf(is not NULL) and buf_sz(>0) must be effective.
 */
i8_t http_object_reset_buffer(http_object_p p_obj, i8_p obj_buf, u32_t buf_sz);

i8_t http_object_add_header(http_object_p p_obj, u8_t header_id, i8_p header, u16_t header_len);

i8_t http_object_add_content(http_object_p p_obj, i8_p content, u16_t content_len);

i8_t http_object_reset(http_object_p p_obj);

i8_t http_object_destroy(http_object_p p_obj);

void http_object_print(http_object_p p_obj);

/*
 * get the http headers, and copy headers into buf
 */
i8_t http_object_get_headers(http_object_p p_obj, i8_p buf, u32_p buf_sz);

/*
 * get one specified header, copy this header into buf
 */
i8_t http_object_get_header(http_object_p p_obj, HTTP_HDR_E header_id, i8_p buf, u32_p buf_size); 

/*
 * get http headers and http content from file
 */
i8_t http_object_parse_file(http_object_p p_obj, string_t file);

/*
 * get http headers and http content from one buffer
 */
i8_t http_object_parse_data(http_object_p p_obj, i8_p data, u16_t data_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_HTTP_H
