
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_http.h"

struct _header_data_s {
	u8_t id;
	i8_p data;
} headers[] = {
	{HTTP_HDR_HOST, "www.freeloop.org"},
	{HTTP_HDR_UA, "Mazilla Firefox"},
	{HTTP_HDR_COOKIE, "sn:100"},
	{HTTP_HDR_END, ""},
	{0,NULL}
};

static i8_p content_seg[] = {
	"<html><head></head>",
	"<title>test</title></html>"
};

static i8_t req_line[64] = { 0 };
static i8_t rsp_line[64] = { 0 };

int main(int argc, char *argv[])
{
	u8_t i;
	http_object_p p_ho;

	p_ho = http_object_create(NULL, 0);
	
	if (NULL == p_ho) {
		return F1G_ERR;
	}
	
	memcpy(rsp_line, "HTTP/1.1 200 OK", strlen("HTTP/1.1 200 OK"));	
	http_object_add_header(p_ho, HTTP_HDR_RSP_LINE, rsp_line, strlen(rsp_line));
	for (i=0; NULL!=headers[i].data; i++) {
		http_object_add_header(p_ho, headers[i].id, headers[i].data, strlen(headers[i].data));	
	}

	http_object_add_content(p_ho, content_seg[0], strlen(content_seg[0]));
	http_object_add_content(p_ho, content_seg[1], strlen(content_seg[1]));

	http_object_print(p_ho);

	http_object_destroy(p_ho);
	p_ho = NULL;

	return F1G_OK;
}
