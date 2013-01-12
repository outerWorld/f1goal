
#ifndef __F1G_HTTP_PARSER_H
#define __F1G_HTTP_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "f1g_basic_types.h"
#include "f1g_string.h"
#include "f1g_bit_array.h"

enum {
	METHOD_GET = 0x00,
	METHOD_POST,
	METHOD_PUT,
	METHOD_OPTIONS,
	METHOD_HEAD,
	METHOD_DELETE,
	METHOD_TRACE,
	METHOD_NUM
};
#define METHOD_GET_STR "GET"
#define METHOD_GET_SZ (sizeof("GET")-1)

#define CR_STR		"\r\n"
#define CR_LEN		(sizeof("\r\n")-1)
#define SP_STR	" "
#define SP_CHAR ' '
//seperator of http header key-value, for example "User-Agent: Mazilla Firefox"
#define HDR_KV_SEP	":"
#define HDR_KV_SEP_SZ (sizeof(":")-1)

#define HTTPV11 0x11
#define HTTPV11_STR	"HTTP/1.1"
#define HTTPV10 0x10
#define HTTPV10_STR	"HTTP/1.0"

typedef c_string_t http_header_string_t;

typedef struct _http_comm_header_s {
	u8_t	version;
	http_header_string_t connection;
}http_comm_header_t, *http_comm_header_p;

typedef struct _http_req_header_s {
	http_comm_header_t 		comm;
	bit_array_t			 	status;
	u8_t				 	method;
	http_header_string_t 	host;
	http_header_string_t 	uri;
	http_header_string_t 	ua;
	http_header_string_t 	cookie;
	http_header_string_t 	refer_uri;
	http_header_string_t 	refer_host;
	http_header_string_t 	accept;
	http_header_string_t 	accept_charset;
	http_header_string_t 	accept_encode;
	http_header_string_t 	accept_lang;
}http_req_header_t, *http_req_header_p;

typedef struct _http_res_header_s {
	http_comm_header_t 		comm;
	bit_array_t				status;
	u8_t					res_code;
	http_header_string_t 	expires;
	http_header_string_t 	date;
	http_header_string_t 	cache_control;
	http_header_string_t 	keepalive;
	http_header_string_t 	content_type;
	http_header_string_t 	content_len;
	http_header_string_t 	pragma;
}http_res_header_t, *http_res_header_p;

i8_t http_req_header_parser(http_req_header_p p_req, const i8_p p_data, const i32_t data_len);

i32_t http_req_header_clear(http_req_header_p p_req);

i8_t http_req_header_print(http_req_header_p p_req);

i8_t http_res_header_parser(http_res_header_p p_res, const i8_p p_data, const data_len);

i8_t http_res_header_print(http_res_header_p p_res);

#ifdef __cplusplus
}
#endif 
#endif // __F1G_HTTP_PARSER_H
