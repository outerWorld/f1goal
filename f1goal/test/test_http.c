
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_http.h"
#include "f1g_http_parser.h"

struct _header_data_s {
	u8_t id;
	i8_p data;
} headers[] = {
	{HTTP_HDR_HOST, "www.freeloop.org"},
	{HTTP_HDR_UA, "Mazilla Firefox"},
	{HTTP_HDR_REFER, "www.google.com/test.c"},
	{HTTP_HDR_COOKIE, "sn:100abcdefghijklmnopqrst"},
	//{HTTP_HDR_END, ""},
	{0,NULL}
};

static i8_p content_seg[] = {
	"<html><head></head>",
	"<title>test</title></html>"
};

static i8_t req_line[64] = { 0 };
static i8_t rsp_line[64] = { 0 };

/*
 * test_http type
 *    type, 0:http request, 1:http response
 */
int main(int argc, char *argv[])
{
	u8_t i;
	i8_t type = 0;
	http_object_p p_ho;
	http_req_header_t req;

	if (argc > 1) {
		type = atoi(argv[1]);
	} 

	p_ho = http_object_create(NULL, 0);
	
	if (NULL == p_ho) {
		return F1G_ERR;
	}
	
	if (type == 1) {
		memcpy(rsp_line, "HTTP/1.1 200 OK", strlen("HTTP/1.1 200 OK"));	
		http_object_add_header(p_ho, HTTP_HDR_RSP_LINE, rsp_line, strlen(rsp_line));
	} else if (type == 0) {
		memcpy(req_line, "GET /test.o HTTP/1.1", strlen("GET /test.o HTTP/1.1"));	
		http_object_add_header(p_ho, HTTP_HDR_REQ_LINE, req_line, strlen(req_line));
	} else {
		fprintf(stderr, "Type=%d is not supported by this test\n", type);
		return 0;
	}
	for (i=0; NULL!=headers[i].data; i++) {
		http_object_add_header(p_ho, headers[i].id, headers[i].data, strlen(headers[i].data));	
	}

/*
	http_object_add_content(p_ho, content_seg[0], strlen(content_seg[0]));
	http_object_add_content(p_ho, content_seg[1], strlen(content_seg[1]));
*/
	http_object_print(p_ho);
	
	p_ho->data_len -= 6;
	memset(&req, 0x00, sizeof(http_req_header_t));
	http_req_header_parser(&req, p_ho->obj_buf, p_ho->data_len);
	http_req_header_print(&req);

	http_object_destroy(p_ho);
	p_ho = NULL;

	return F1G_OK;
}
