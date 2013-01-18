
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_basic_funcs.h"
#include "f1g_regset.h"

i32_t regset_init(regset_p p_rs, i32_t rule_size)
{
	i32_t i = 0;

	if (rule_size <= 0) rule_size = 10;

	MEM_ALOC(p_rs->p_rules, reg_rule_p, rule_size*sizeof(reg_rule_t), F1G_ERR);

	// Initialize each reg_rule
	// Not do it now, just see its possibility in application

	return F1G_OK;
}

regset_p regset_create(i32_t rule_size)
{
	regset_p p_rs;

	MEM_ALOC(p_rs, regset_p, sizeof(regset_t), NULL);

	if (F1G_OK != regset_init(p_rs, rule_size)) {
		MEM_FREE(p_rs);
		return NULL;
	}

	return p_rs;
}

i32_t regset_addreg(regset_p p_rs, string_t reg_rule, string_t value, reg_proc_f_p p_regf)
{
	i32_t len = 0;
	reg_rule_p p_cur =  0;

	if (p_rs->rule_num >= p_rs->rule_size) {
		regset_enlarge_cap(p_rs, p_rs->rule_size + 10);
	}

	p_cur = &p_rs->p_rules[p_rs->rule_num];
	
	len = strlen(reg_rule);
	if (F1G_OK != buffer_init(&p_cur->rule, len + 64)) {
		return F1G_ERR;
	}

	if (F1G_OK != buffer_append(&p_cur->rule, reg_rule, len)) {
		return F1G_ERR;
	}

	len = strlen(value);
	if (F1G_OK != buffer_init(&p_cur->rule, len + 64)) {
		return F1G_ERR;
	}

	if (F1G_OK != buffer_append(&p_cur->rule, value, len)) {
		return F1G_ERR;
	}
	
	buffer_init(&p_cur->out_buf, 0);
	p_cur->p_f = p_regf;

	p_rs->rule_num ++;

	return F1G_OK;
}

i32_t regset_delreg(regset_p p_rs, string_t reg_rule)
{
	return F1G_OK;
}

i32_t regset_match(regset_p p_rs, string_t in_data, string_t * out_data)
{
	i32_t i;
	reg_rule_p p_cur;

	*out_data = NULL;
	for (i=0; i<p_rs->rule_num; i++) {
		p_cur = &p_rs->p_rules[i];
		// basic match method, later more will be added.
		if (NULL != strstr(in_data, buffer_data(&p_cur->rule))) {
			*out_data = buffer_data(&p_cur->reg_value);
			break;
		}
	}

	return F1G_ERR;
}

i32_t regset_destroy(regset_p p_rs)
{
	return F1G_OK;
}

i32_t regset_show(regset_p p_rs)
{
	i32_t i;

	fprintf(stdout, "regset:\n\trule_size = %d, rule_num = %d\n",
			p_rs->rule_size, p_rs->rule_num);	

	for (i=0; i<p_rs->rule_num; i++) {
		fprintf(stdout, "\trule: ");	
		buffer_show(&p_rs->p_rules[i].rule);
		fprintf(stdout, "\treg_value: ");
		buffer_show(&p_rs->p_rules[i].reg_value);
		fprintf(stdout, "\tout_buf: ");
		buffer_show(&p_rs->p_rules[i].out_buf);
		fprintf(stdout, "\tproc_func: %p\n", p_rs->p_rules[i].p_f);
	}

	return F1G_OK;
}

