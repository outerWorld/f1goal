
#ifndef __F1G_CMD_LINE_H
#define __F1G_CMD_LINE_H

#include "f1g_basic_types.h"

#define PARA_NAME_SZ	64
typedef struct _cmd_para_s {
	u8_t	para_name[PARA_NAME_SZ];
	u8_t 	type;
	void *	value;
}cmd_para_t, *cmd_para_p;

typedef struct _cmd_line_obj_s {
	u8_t 		para_num;
	cmd_para_p 	p_paras;
}cmd_line_obj_t, *cmd_line_obj_p;

cmd_line_obj_p cmd_line_obj_create(u8_p cmd_name, i32_t para_num);
i32_t cmd_line_reg_para(u8_p para_name, u8_t type, void *def_value);
i32_t cmd_line_parse(i32_t argc, u8_p argv[], cmd_line_obj_p p_obj);
void *cmd_line_get_para_value(cmd_line_obj_p p_obj, u8_p para_name);
u8_p cmd_line_get_para(cmd_line_obj_p p_obj, u32_t para_seq);
i32_t cmd_line_obj_destroy(cmd_line_obj_p p_obj);

#endif
