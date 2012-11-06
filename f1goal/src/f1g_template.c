
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "f1g_template.h"

i32_t f1g_template_init(f1g_template_p p_tpl, i32_t ini_node_size)
{
	node_info_p p_nodes = NULL;

	memset(p_tpl, 0x00, sizeof(f1g_template_t));
	if (ini_node_size > 0) {
		p_tpl->node_size = ini_node_size;
		p_nodes = (node_info_p)malloc(sizeof(node_info_t)*ini_node_size);		
		if (NULL == p_nodes) {
			return F1G_ERR;
		}
		memset(p_nodes, 0x00, sizeof(node_info_t)*ini_node_size);
		p_tpl->p_nodes = p_nodes;
	}
	
	p_tpl->parse_status = PARSE_INIT;

	return F1G_OK;
}

i32_t f1g_template_print(f1g_template_p p_tpl)
{
	i32_t i;
	node_info_p p_node = NULL;

	fprintf(stdout, "=======================================\n");
	fprintf(stdout, "\tTemplate name [%s]\n", p_tpl->name);
	fprintf(stdout, "\t  Node size [%d], Node number [%d]\n", p_tpl->node_size, p_tpl->node_num);	
	for (i=0; i<p_tpl->node_num; i++) {
		p_node = &p_tpl->p_nodes[i];
		fprintf(stdout, "\t  Node[%d]: name[%s], node type [%d], var type [%d], data len [%d], data [%.*s]\n", i, p_node->var_name, p_node->node_type, p_node->var_type, p_node->data_len, p_node->data_len, p_node->data);
	}
	fprintf(stdout, "\t  Parse Status: [%d]\n", p_tpl->parse_status);
	
	fprintf(stdout, "=======================================\n");

	return F1G_OK;
}

static i32_t _check_syntax_chars(char ch)
{
	if ((ch>=0x30 && ch<=0x39) || (ch>=0x41 && ch<=0x5A) || (ch>=0x61 && ch<=0x7A)) {
		return 0;
	}
	switch(ch) {
		case '_':
		case '[':
		case ']':
		case '-':
		case '.':
		case '*':
		case '@':
			return 0;
		default:
			return 1;
	}

	return 1;
}

enum {
	RST_FOUND_VAR = 0x00,
	RST_FOUND_NONE,
};
static i32_t find_variable(i8_p pb, i8_p * pe, i8_p *p_vtag_b, i8_p *p_vtag_e, i8_t *result)
{
	i8_t var_flag_found = 0;	// it means '$' has been found
	i8_t prefix_found = 0;		// it means '${' has been found
	i8_t var_found = 0;
	i8_p p_var_flag = NULL;

	*result = RST_FOUND_NONE;
	while (pb!=*pe && 1!=var_found) {
		switch (*pb) {
			case VAR_FLAG:
				if (0 != var_flag_found) {
					// Escape characters: '$$' --> '$'
					if (pb == p_var_flag+1) {
						// move and cover.
						memcpy(p_var_flag, pb, (*pe)-pb);
						(*pe)--;
						var_flag_found = 0;
						p_var_flag = NULL;
					} else {
						// Syntax Error, Not supported variable name:'${' has already been found
						if (1 == prefix_found) {
							*p_vtag_b = NULL;
							return F1G_ERR;
						} else {
							var_flag_found = 1;
							p_var_flag = pb;
							pb++;
						}
					}
				} else {
					var_flag_found = 1;
					p_var_flag = pb;
					pb++;
				}
				break;
			case SPLIT_BEG:
				if (1 == var_flag_found) {
					if (pb-p_var_flag == 1) {
						*p_vtag_b = pb;
						prefix_found = 1;
					} else {
						if (1 == prefix_found) {
							// Syntax Error, Not supported variable name: '${' has been found
							return F1G_ERR;
						} else {
							var_flag_found = 0;
							p_var_flag = NULL;
						}
					}
				} else {
					// TODO nothing.
				}
				pb++;
				break;
			case SPLIT_END:
				if (1 == prefix_found) {
					// Varible name is empty, Syntax Error!
					if (pb-*p_vtag_b == 1) {
						return F1G_ERR;
					} else {
						*p_vtag_e = pb;
						var_found = 1;
						*result = RST_FOUND_VAR;
					}
				} else {
					// TODO nothing.
				}
				pb++;
				break;
			default:
				if (1 == prefix_found) {
					if (_check_syntax_chars(*pb) != 0) {
						return F1G_ERR;
					} else {
						// TODO nothing.
					}
				} else {
					// TODO nothing.
				}
				pb++;
				break;
		}
	}

	return F1G_OK;
}

i32_t f1g_template_parse(f1g_template_p p_tpl, string_t file)
{
	i32_t fd;
	i32_t ret;
	i8_t  var_found = 0;
	struct stat st;
	i8_t * pb, *pe, * p_vtag_b, *p_vtag_e;

	fd = open(file, O_RDONLY, 0);
	if (fd < 0) {
		p_tpl->parse_status = PARSE_ERR;
		return F1G_ERR;
	}

	if (fstat(fd, &st) != 0) {
		p_tpl->parse_status = PARSE_ERR;
		close(fd);
		return F1G_ERR;
	}
	
	if (st.st_size <= 0) {
		p_tpl->parse_status = PARSE_ERR;
		close(fd);
		return F1G_ERR;
	}

	p_tpl->buf = (i8_p)malloc(st.st_size);
	if (NULL == p_tpl->buf) {
		p_tpl->parse_status = PARSE_ERR;
		close(fd);
		return F1G_ERR;
	}
	memset(p_tpl->buf, 0x00, st.st_size);

	if (read(fd, p_tpl->buf, st.st_size) != st.st_size) {
		close(fd);
		free(p_tpl->buf);
		p_tpl->buf = NULL;
		p_tpl->parse_status = PARSE_ERR;
		return F1G_ERR;
	}
	p_tpl->data_len = st.st_size;
	p_tpl->buf_size = st.st_size;
	close(fd);

	pb = p_tpl->buf;
	pe = pb + p_tpl->data_len;

	while (pb < pe) {
		p_vtag_b = NULL;
		p_vtag_e = NULL;
		if (F1G_OK != find_variable(pb, &pe, &p_vtag_b, &p_vtag_e, &var_found)) {
			p_tpl->parse_status = PARSE_ERR;
			return F1G_ERR;
		}
		
		if (RST_FOUND_NONE == var_found) {
			p_vtag_e = pe;
			p_vtag_b = p_vtag_e;
		}
		
		if (p_vtag_b-pb-1 > 0) {
			ret = f1g_template_add_node(p_tpl, NODE_TYPE_BLK_DATA, NULL, 0, pb, p_vtag_b-pb-1);
			if (F1G_OK != ret) {
				p_tpl->parse_status = PARSE_ERR;
				return F1G_ERR;
			}
		}

		if (RST_FOUND_VAR == var_found) {
			if (p_vtag_e-p_vtag_b-1 > MAX_VAR_LEN) {
				p_tpl->parse_status = PARSE_ERR;
				return F1G_ERR;
			}
	
			ret = f1g_template_add_node(p_tpl, NODE_TYPE_VAR, p_vtag_b+1, p_vtag_e-p_vtag_b-1, NULL, 0);
			if (F1G_OK != ret) {
				p_tpl->parse_status = PARSE_ERR;
				return F1G_ERR;
			}
		}

		pb = p_vtag_e + 1;
	}
	
	p_tpl->parse_status = PARSE_OK;

	return F1G_OK;
}

i32_t f1g_template_exec(f1g_template_p p_tpl, i8_p p_buf, i32_t buf_size, data_set_p ds)
{
	return F1G_OK;
}

i32_t f1g_template_extent_cap(f1g_template_p p_tpl, i32_t ext_node_size)
{
	node_info_p p_tmp_nodes = NULL;
	
	p_tmp_nodes = p_tpl->p_nodes;

	p_tpl->p_nodes = (node_info_p)malloc(ext_node_size);
	if (NULL == p_tpl->p_nodes) {
		p_tpl->p_nodes = p_tmp_nodes;
		return F1G_ERR;
	}
	memset(&p_tpl->p_nodes, 0x00, sizeof(node_info_t)*ext_node_size);
	if (p_tmp_nodes) {
		memcpy(p_tpl->p_nodes, p_tmp_nodes, p_tpl->node_num*sizeof(node_info_t));
		free(p_tmp_nodes);
	}
	p_tpl->node_size = ext_node_size;

	return F1G_OK;
}

i32_t f1g_template_add_node(f1g_template_p p_tpl, u8_t node_type, i8_p var_name, u8_t var_name_len, i8_p p_data, i32_t data_len)
{
	node_info_p p_node = NULL;

	if (p_tpl->node_num >= p_tpl->node_size) {
		if (F1G_OK != f1g_template_extent_cap(p_tpl, p_tpl->node_size*2)) {
			return F1G_ERR;
		}
	}

	p_node = &p_tpl->p_nodes[p_tpl->node_num];
	p_node->node_type = node_type;
	if (var_name_len>0 && var_name_len<=MAX_VAR_LEN) {
		memcpy(p_node->var_name, var_name, var_name_len);
		p_node->var_name[var_name_len] = '\0';
	}
	
	p_node->data = p_data;
	p_node->data_len = data_len;

	p_tpl->node_num++;

	return F1G_OK;
}

i32_t f1g_template_destroy(f1g_template_p p_tpl)
{
	if (NULL != p_tpl) {
		if (NULL!=p_tpl->p_nodes && p_tpl->node_size>0) {
			free(p_tpl->p_nodes);
		}
		
		if (NULL!=p_tpl->buf && p_tpl->buf_size>0) {
			free(p_tpl->buf);
		}

		memset(p_tpl, 0x00, sizeof(f1g_template_t));
	}
	return F1G_OK;
}

