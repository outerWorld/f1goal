/*
 * Usage : regular match set
 * Author: huang chunping
 * Date  : 2013-01-18
 * Plan  : 1) the first step is to complete exact match and substr match(maybe mostly used)
 */

#ifndef __F1G_REGSET_H
#define __F1G_REGSET_H

#include "f1g_basic_types.h"
#include "f1g_buffer.h"

typedef i32_t (*reg_proc_f_p)(string_t in_data, string_t * out_data);

typedef struct _reg_rule_s {
	buffer_t		rule; 		// example: *.abc 
	buffer_t 		reg_value; 	// as returned value if matched
	buffer_t 		out_buf;    // as parameter of p_f
	reg_proc_f_p 	p_f;		// if matched ,this function is called.
}reg_rule_t, *reg_rule_p;

// regular matching set
typedef struct _regset_s {
	i32_t 		rule_num;
	i32_t 		rule_size;
	reg_rule_p 	p_rules;
}regset_t, *regset_p;

i32_t regset_init(regset_p p_rs, i32_t rule_size);

regset_p regset_create(i32_t rule_size);

i32_t regset_addreg(regset_p p_rs, string_t reg_rule, string_t value, reg_proc_f_p p_regf);

i32_t regset_delreg(regset_p p_rs, string_t reg_rule);

i32_t regset_match(regset_p p_rs, string_t in_data, string_t * out_data);

i32_t regset_destroy(regset_p p_rs);

i32_t regset_show(regset_p p_rs);

#endif // __F1G_REGSET_H
