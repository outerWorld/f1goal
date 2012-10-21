
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_basic_funcs.h"
#include "f1g_http_parser.h"

typedef struct _http_method_s {
	u8_t 		id;
	u8_t		method_len;
	string_t	method;
}http_method_t, *http_method_p;

static http_method_t sg_methods[] = {
	{METHOD_GET, 		3, "GET"},
	{METHOD_POST, 		4, "POST"},
	{METHOD_PUT, 		3, "PUT"},
	{METHOD_OPTIONS, 	7, "OPTIONS"},
	{METHOD_HEAD, 		4, "HEAD"},
	{METHOD_DELETE, 	6, "DELETE"},
	{METHOD_TRACE, 		5, "TRACE"},
	{METHOD_NUM, 		-1, ""},
};

typedef struct _http_header_info_s {
	u8_t 		id;
	u8_t 		len;
	string_t 	header;
}http_header_info_t, *http_header_info_p;

#define HOST_STR 	"Host"
#define UA_STR		"User-Agent"
#define COOKIE_STR 	"Cookie"
#define REFER_STR	"Referer"
#define ACCEPT_STR	"Accept"
#define ACC_CHARSET_STR	"Accept-Charset"
#define ACC_ENCODE_STR	"Accept-Encoding"
#define ACC_LANG_STR	"Accept-Language"
#if 0
static http_header_info_t sg_headers[] = {
	{HTTP_HDR_HOST, sizeof(HOST_STR)-1, HOST_STR},
	{HTTP_HDR_UA, sizeof(UA_STR)-1, UA_STR},
	{HTTP_HDR_COOKIE, sizeof(COOKIE_STR)-1, COOKIE_STR},
	{HTTP_HDR_REFER, sizeof(REFER_STR)-1, REFER_STR},
	{HTTP_HDR_ACCEPT, sizeof(ACCEPT_STR)-1, ACCEPT_STR},
	{HTTP_HDR_ACC_CHARSET, sizeof(ACC_CHARSET_STR)-1, ACC_CHARSET_STR},
	{HTTP_HDR_ACC_ENCODE, sizeof(ACC_ENCODE_STR)-1, ACC_ENCODE_STR},
	{HTTP_HDR_ACC_LANG, sizeof(ACC_LANG_STR)-1, ACC_LANG_STR},
	{HTTP_HDR_NUM, 0, ""},
	
};
#endif

static i8_t add_header(http_req_header_p p_req, i8_p p_key1, i8_p p_key1_e, i8_p p_key2, i8_p p_key2_e)
{
	i8_p ptmp = NULL;

	switch (*p_key1) {
		case 'A':
			if (cmp_str2(p_key1, p_key1_e-p_key1+1, ACC_LANG_STR, sizeof(ACC_LANG_STR)-1) == 0) {
				c_string_set(&p_req->accept_lang, p_key2, p_key2_e-p_key2+1);
			} else if (cmp_str2(p_key1, p_key1_e-p_key1+1, ACC_ENCODE_STR, sizeof(ACC_ENCODE_STR)-1) == 0) {
				c_string_set(&p_req->accept_encode, p_key2, p_key2_e-p_key2+1);
			} else if (cmp_str2(p_key1, p_key1_e-p_key1+1, ACC_CHARSET_STR, sizeof(ACC_CHARSET_STR)-1) == 0) {
				c_string_set(&p_req->accept_charset, p_key2, p_key2_e-p_key2+1);
			} else if (cmp_str2(p_key1, p_key1_e-p_key1+1, ACCEPT_STR, sizeof(ACCEPT_STR)-1) == 0) {
				c_string_set(&p_req->accept, p_key2, p_key2_e-p_key2+1);
			} else {
				// TODO nothing.
			}
			break;
		case 'C':
			if (cmp_str2(p_key1, p_key1_e-p_key1+1, COOKIE_STR, sizeof(COOKIE_STR)-1) == 0) {
				c_string_set(&p_req->cookie, p_key2, p_key2_e-p_key2+1);
			} else {
				// TODO nothing.
			}
			break;
		case 'H':
			if (cmp_str2(p_key1, p_key1_e-p_key1+1, HOST_STR, sizeof(HOST_STR)-1) == 0) {
				c_string_set(&p_req->host, p_key2, p_key2_e-p_key2+1);
			} else {
				// TODO nothing.
			}
			break;
		case 'R':
			if (cmp_str2(p_key1, p_key1_e-p_key1+1, REFER_STR, sizeof(REFER_STR)-1) == 0) {
				find_str(p_key2, p_key2_e, "/", &ptmp);
				if (ptmp == p_key2 ) {
					// TODO nothing.
				}else if (NULL == ptmp) {
					return F1G_ERR;
				}
				c_string_set(&p_req->refer_host, p_key2, ptmp-p_key2);
				c_string_set(&p_req->refer_uri, ptmp, p_key2_e-ptmp+1);
			} else {
			}
			break;
		case 'U':
			if (cmp_str2(p_key1, p_key1_e-p_key1+1, UA_STR, sizeof(UA_STR)-1) == 0) {
				c_string_set(&p_req->ua, p_key2, p_key2_e-p_key2+1);
		} else {
		}
			break;
		default:
			break;
	}

	return F1G_OK;
}

i8_t http_req_header_parser(http_req_header_p p_req, const i8_p p_data, const i32_t data_len)
{
	i8_t flag = 0;
	i8_p p_line_b = NULL;
	i8_p p_line_e = NULL;
	i8_p p_key1_b  = NULL;
	i8_p p_key1_e  = NULL;
	i8_p p_key2_b  = NULL;
	i8_p p_key2_e  = NULL;
	i8_p p_data_end = p_data + data_len - 1;
	i32_t line_len = 0;

	// Get and parse request line
	p_line_b = p_data;
	find_str(p_line_b, p_data_end, CR_STR, &p_line_e);
	if (p_line_e == NULL || p_line_e==p_line_b) {
		return F1G_ERR;
	}
	// GET method
	p_key1_b = p_line_b;
	if (cmp_str2(p_key1_b, p_line_e-p_line_b, METHOD_GET_STR, METHOD_GET_SZ) != 0) {
		return F1G_ERR;
	}
	p_req->method = METHOD_GET;

	// GET uri
	p_key1_b += METHOD_GET_SZ;
	skip_space(&p_key1_b, p_line_e-p_key1_b);
	find_str(p_key1_b, p_line_e-1, SP_STR, &p_key1_e);
	if (p_key1_e==NULL || p_key1_e==p_key1_b) {
		return F1G_ERR;
	}
	c_string_set(&p_req->uri, p_key1_b, p_key1_e-p_key1_b);
	p_key1_b += (p_key1_e-p_key1_b);
	skip_space(&p_key1_b, p_line_e-p_key1_b);

	// GET version
	if (cmp_str2(p_key1_b, p_line_e-p_key1_b, HTTPV11_STR, sizeof(HTTPV11_STR)-1) == 0) {
		p_req->comm.version = HTTPV11;
	} else if (cmp_str2(p_key1_b, p_line_e-p_key1_b, HTTPV10_STR, sizeof(HTTPV10_STR)-1) == 0) {
		p_req->comm.version = HTTPV10;
	} else {
		// unrecognized version.
		return F1G_ERR;
	}
	
	flag = 1;
	while (flag) {
		p_line_b = p_line_e + CR_LEN;
		skip_space(&p_line_b, p_data_end-p_line_b);
		find_str(p_line_b, p_data_end, CR_STR, &p_line_e);
		if (NULL == p_line_e) {
			flag = 0;
			p_line_e = p_data_end;
		} else {
			//p_line_e--;
		}
		if (p_line_e <= p_line_b) {
			break;
		}

		p_key1_b = p_line_b;
		find_str(p_key1_b, p_line_e, HDR_KV_SEP, &p_key1_e);
		if (NULL==p_key1_e || p_key1_e==p_key1_b) {
			break;
		}
		p_key2_b = p_key1_e + HDR_KV_SEP_SZ;
		skip_space(&p_key2_b, p_line_e-p_key2_b);
		strip_char(p_key2_b, p_line_e-p_key2_b, SP_CHAR, &p_key2_e);
		add_header(p_req, p_key1_b, p_key1_e, p_key2_b, p_key2_e);
	}

	return F1G_OK;
}

i8_t http_res_header_parser(http_res_header_p p_res, const i8_p p_data, const data_len)
{
	return F1G_OK;
}

i8_t http_req_header_print(http_req_header_p p_req)
{
	fprintf(stdout, "METHOD=%02x|VERSION=%02x\n", p_req->method, p_req->comm.version);
	fprintf(stdout, "%s|%.*s\n", HOST_STR, p_req->host.len, p_req->host.data);
	fprintf(stdout, "%s|%.*s\n", "Uri", p_req->uri.len, p_req->uri.data);
	fprintf(stdout, "%s|%.*s\n", COOKIE_STR, p_req->cookie.len, p_req->cookie.data);
	fprintf(stdout, "%s|%.*s\n", UA_STR, p_req->ua.len, p_req->ua.data);
	fprintf(stdout, "%s|%.*s|%.*s\n", REFER_STR, p_req->refer_host.len, p_req->refer_host.data, p_req->refer_uri.len, p_req->refer_uri.data);
	fprintf(stdout, "%s|%.*s\n", ACCEPT_STR, p_req->accept.len, p_req->accept.data);
	fprintf(stdout, "%s|%.*s\n", ACC_LANG_STR, p_req->accept_lang.len, p_req->accept_lang.data);
	fprintf(stdout, "%s|%.*s\n", ACC_ENCODE_STR, p_req->accept_encode.len, p_req->accept_encode.data);
	fprintf(stdout, "%s|%.*s\n", ACC_CHARSET_STR, p_req->accept_charset.len, p_req->accept_charset.data);

	return F1G_OK;
}

i8_t http_res_header_print(http_res_header_p p_res)
{
	return F1G_OK;
}
