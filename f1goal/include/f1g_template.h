
#ifndef __F1G_TEMPLATE_H
#define __F1G_TEMPLATE_H

#include "f1g_basic_types.h"
#include "f1g_data_set.h"

// byte size of template name
#define TPL_NAME_SIZE	64

#define MAX_VAR_LEN	64
#define MIN_VAR_LEN	1

#define VAR_FLAG	'$'
#define SPLIT_BEG	'{'
#define SPLIT_END	'}'

enum {
	NODE_TYPE_VAR = 0x00,	// node type is variable.
	NODE_TYPE_BLK_DATA,		// node type is block data.
};

enum {
	VAR_TYPE_INT = 0x00,
	VAR_TYPE_STRING
};

enum {
	PARSE_INIT = 0x00,
	PARSE_OK,
	PARSE_ERR,
};

enum {
	EXEC_INIT = 0x00,
	EXEC_OK,
	EXEC_BUF_NOT_ENOUGH,
};


typedef struct _node_info_s {
	u8_t 	node_type;
	u8_t 	var_type;
	i8_t	var_name[MAX_VAR_LEN+1];
	i32_t	data_len;
	i8_p	data;
}node_info_t, *node_info_p;

typedef struct _f1g_template_s {
	i8_t		name[TPL_NAME_SIZE+1];
	i32_t		node_num;
	i32_t		node_size;
	node_info_p p_nodes;
#ifdef _SINGLE_THREAD_USING
	u8_t		parse_status;
	u8_t		exec_status;
#endif // _SINGLE_THREAD_USING
	i32_t		data_len;
	i32_t 		buf_size;
	i8_p 		buf;
}f1g_template_t, *f1g_template_p;

i32_t f1g_template_init(f1g_template_p p_tpl, i32_t ini_node_size);

i32_t f1g_template_extent_cap(f1g_template_p p_tpl, i32_t ext_node_size);

i32_t f1g_template_print(f1g_template_p p_tpl);

i32_t f1g_template_parse(f1g_template_p p_tpl, string_t file);

i32_t f1g_template_exec(f1g_template_p p_tpl, value_p p_buf, data_set_p ds);

i32_t f1g_template_add_node(f1g_template_p p_tpl, u8_t node_type, i8_p var_name, u8_t var_name_len, i8_p p_data, i32_t data_len);

i32_t f1g_template_destroy(f1g_template_p p_tpl);

#endif // __F1G_TEMPLATE_H
