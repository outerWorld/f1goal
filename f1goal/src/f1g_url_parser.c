
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_url_parser.h"

i32_t url_parser_init(url_parser_p p_url_parser, i32_t param_size)
{
	memset(p_url_parser, 0x00, sizeof(url_parser_t));
}

i32_t url_parser_load(url_parser_p p_url_parser, i8_p p_url, i32_t url_len)
{
	i32_t i = 0;
	i8_p p = p_url;
	i8_p p2 = NULL;
	i32_t new_size = 0;
	c_string_p p_str = NULL;
	i8_p p_url_end = p_url + url_len;

	// strip 'http://'
	p = strstr(p, "http://");
	if (!p) p = p_url;
	else p += sizeof("http://")-1;
	// determine host's existence
	if ('/' == *p) {
		// not exist.
		// do nothing.
	} else {
		p2 = p;
		while (p2 < p_url_end && '/' != *p2) p2++;
		// url is abnormal.
		if (p2 >= p_url_end) return F1G_ERR;
		p_url_parser->host.data = p;
		p_url_parser->host.len = p2 - p;
		// move to the position of path
		p = p2;
	}

	p2 = p_url_end - 1;
	// end to front loop, in the previous logic, it promises *p equals '/',
	// so p2 must be equal to or larger than p after the loop ends
	while (p2 > p && '/' != *p2) p2--;
	p2++;
	p_url_parser->uri_path.data = p;
	p_url_parser->uri_path.len = p2 - p;	
	
	// to get resource
	p = p2;
	while (p2 < p_url_end && '?' != *p2) p2++;
	if (p2 - p <= 0) {
		return F1G_OK;
	}
	p_url_parser->res_name.data = p;
	p_url_parser->res_name.len = p2 - p;
	// get resource type
	while (p < p2 && '.' != *p) p++;
	if (p2 - p > 0) {
		p++; //skip '.'
		p_url_parser->res_type.data = p;
		p_url_parser->res_type.len = p2 - p;
	}

	// move to position of parameters, parameters format: para1=4&para2=string
	// skip '?'
	p = p2 = p2 + 1;
	
	while (p2 < p_url_end) {
		if (i >= p_url_parser->uri_param_size) {
			// extend param capacity.
			new_size = i + 12;
			p_str = p_url_parser->uri_params_name;
			p_str = (c_string_p)malloc(sizeof(c_string_t)*new_size*2);
			if (!p_str) return F1G_ERR;
			if (p_url_parser->uri_param_num > 0) {
				memcpy(p_str, p_url_parser->uri_params_name, p_url_parser->uri_param_num*sizeof(c_string_t));
				memcpy(p_str + sizeof(c_string_t)*new_size, p_url_parser->uri_params_value, p_url_parser->uri_param_num*sizeof(c_string_t));
			}
			if (p_url_parser->uri_params_name) free(p_url_parser->uri_params_name);
			if (p_url_parser->uri_params_value) free(p_url_parser->uri_params_value);
			p_url_parser->uri_params_name = p_str;
			p_url_parser->uri_params_value = p_str + sizeof(c_string_t)*new_size;
			p_url_parser->uri_param_size = new_size;
		}

		// get para name.
		while (p2 < p_url_end && '=' != *p2) p2++;
		p_url_parser->uri_params_name[i].data = p;
		p_url_parser->uri_params_name[i].len = p2 - p;
		if (p2 >= p_url_end) continue;

		// get para value.
		p = p2 = p2 + 1; // skip '='
		while (p2 < p_url_end &&  '&' != *p2) p2++;
		p_url_parser->uri_params_value[i].data = p;
		p_url_parser->uri_params_value[i].len = p2 - p;
		p = p2 = p2 + 1; // skip '&'
		i++;
	}
	p_url_parser->uri_param_num = i;

	return F1G_OK;
}

i32_t url_parser_show(url_parser_p p_url_parser)
{
	i32_t i;

	fprintf(stdout, "host:%.*s, res_name:%.*s, res_type:%.*s, uri_path:%.*s\n", 
			p_url_parser->host.len, p_url_parser->host.data, p_url_parser->res_name.len, p_url_parser->res_name.data, 
			p_url_parser->res_type.len, p_url_parser->res_type.data, p_url_parser->uri_path.len, p_url_parser->uri_path.data);
	for (i=0; i<p_url_parser->uri_param_num; i++) {
		fprintf(stdout, "%.*s:%.*s;", p_url_parser->uri_params_name[i].len, p_url_parser->uri_params_name[i].data, p_url_parser->uri_params_value[i].len, p_url_parser->uri_params_value[i].data);
	}
	fprintf(stdout, "\n");

	return F1G_OK;
}

i32_t url_parser_clear(url_parser_p p_url_parser)
{
	i32_t i = 0;
	
	p_url_parser->uri_param_num = 0;
	c_string_set(&p_url_parser->host, 0, 0);	
	c_string_set(&p_url_parser->uri_path, 0, 0);	
	c_string_set(&p_url_parser->res_type, 0, 0);	
	c_string_set(&p_url_parser->res_name, 0, 0);	
	memset(p_url_parser->uri_params_name, 0x00, sizeof(c_string_t)*p_url_parser->uri_param_num);
	memset(p_url_parser->uri_params_value, 0x00, sizeof(c_string_t)*p_url_parser->uri_param_num);

	return F1G_OK;
}

i32_t url_parser_destroy(url_parser_p p_url_parser)
{
	// note that, uri_params_name and uri_params_value are at a continous memory whose start address is pointed by uri_params_name, so free uri_params_name is enough.
	if (p_url_parser->uri_params_name) {
		free(p_url_parser->uri_params_name);
	}
	memset(p_url_parser, 0x00, sizeof(url_parser_t));

	return F1G_OK;
}

