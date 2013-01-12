/*
 * Author: huang chunping
 * Date  : 2013-01-12
 */
#ifndef __F1G_URL_PARSER_H
#define __F1G_URL_PARSER_H

#include "f1g_basic_types.h"
#include "f1g_string.h"

typedef struct _url_parser_s {
	c_string_t host;
	c_string_t uri_path;
	c_string_t res_type; // resource type:html,htm,jpg,gif
	c_string_t res_name; // ex. index.html
	i32_t	   uri_param_num;
	i32_t	   uri_param_size;
	c_string_p uri_params_name;
	c_string_p uri_params_value;
}url_parser_t, *url_parser_p;

i32_t url_parser_init(url_parser_p p_url_parser, i32_t param_size);

i32_t url_parser_load(url_parser_p p_url_parser, i8_p p_url, i32_t url_len);

i32_t url_parser_show(url_parser_p p_url_parser);
i32_t url_parser_clear(url_parser_p p_url_parser);
i32_t url_parser_destroy(url_parser_p p_url_parser);

#endif // __F1G_URL_PARSER_H
