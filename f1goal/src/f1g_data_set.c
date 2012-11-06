
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_data_set.h"

i32_t key_info_extent_cap(key_info_p p_key, i32_t ext_size)
{
	i8_p p_val;

	p_val = p_key->val;
	p_key->val = (i8_p)malloc(ext_size);
	if (NULL == p_key->val) {
		p_key->val = p_val;
		return F1G_ERR;
	}
	memset(p_key->val, 0x00, ext_size);
	if (p_val) {
		memcpy(p_key->val, p_val, p_key->val_len);
	}
	p_key->val_size = ext_size;

	return F1G_OK;
}

i32_t data_set_init(data_set_p p_ds, i32_t key_size)
{
	i32_t i = 0;

	memset(p_ds, 0x00, sizeof(data_set_t));

	p_ds->p_keys = (key_info_p)malloc(sizeof(key_info_t)*key_size);
	if (NULL == p_ds->p_keys) {
		return F1G_ERR;
	}
	memset(p_ds->p_keys, 0x00, key_size*sizeof(key_info_t));
	p_ds->key_size = key_size;

	for (i=0; i<key_size; i++) {
		p_ds->p_keys[i].val = (i8_p)malloc(KEY_VAL_SIZE);
		if (NULL == p_ds->p_keys[i].val) {
			return F1G_ERR;
		}
		p_ds->p_keys[i].val_size = KEY_VAL_SIZE;
	}

	return F1G_OK;
}

i32_t data_set_extent_cap(data_set_p p_ds, i32_t ext_key_size)
{
	key_info_p p_tmp_keys = NULL;

	p_tmp_keys = p_ds->p_keys;
	p_ds->p_keys = (key_info_p)malloc(sizeof(key_info_t)*ext_key_size);
	if (NULL == p_ds->p_keys) {
		p_ds->p_keys = p_tmp_keys;
		return F1G_ERR;
	}
	memset(p_ds->p_keys, 0x00, ext_key_size*sizeof(key_info_t));

	if (p_tmp_keys) {
		memcpy(p_ds->p_keys, p_tmp_keys, p_ds->key_num*sizeof(key_info_t));
		free(p_tmp_keys);
	}
	p_ds->key_size = ext_key_size;

	return F1G_OK;
}

i32_t data_set_add_int_var(data_set_p p_ds, string_t var_name, void *in_var_val, string_t fmt)
{
	i32_t len = 0;
	key_info_p p_key = NULL;

	if (p_ds->key_num >= p_ds->key_size) {
		data_set_extent_cap(p_ds, p_ds->key_size*2);
	}

	p_key = &p_ds->p_keys[p_ds->key_num];

	if (NULL==fmt || strlen(fmt)<=0) {
		// TODO, not supported now!
	} else {
		len = snprintf(p_key->val, p_key->val_size, fmt, *(long long*)in_var_val);
		if (len > p_key->val_size) {
			key_info_extent_cap(p_key, len+len/2);
		}
		p_key->val_len = snprintf(p_key->val, p_key->val_size, fmt, *(long long*)in_var_val);
	}

	if (strlen(var_name) > MAX_KEY_LEN) {
		return F1G_ERR;	
	}
	strncpy(p_key->key, var_name, MAX_KEY_LEN);
	p_key->key[MAX_KEY_LEN] = '\0';

	p_ds->key_num++;

	return F1G_OK;
}

i32_t data_set_add_str_var(data_set_p p_ds, string_t var_name, void *in_var_val, i32_t val_len)
{
	int len = 0;
	key_info_p p_key = NULL;

	if (p_ds->key_num >= p_ds->key_size) {
		data_set_extent_cap(p_ds, p_ds->key_size*2);
	}
	p_key = &p_ds->p_keys[p_ds->key_num];

	if (p_key->val_size < val_len) {
		key_info_extent_cap(p_key, val_len+val_len/2);
	}
	
	if (strlen(var_name) > MAX_KEY_LEN) {
		return F1G_ERR;	
	}
	strncpy(p_key->key, var_name, MAX_KEY_LEN);
	p_key->key[MAX_KEY_LEN] = '\0';
	memcpy(p_key->val, in_var_val, val_len);
	p_key->val[val_len] = '\0';

	p_ds->key_num++;

	return F1G_OK;
}

i32_t data_set_destroy(data_set_p p_ds)
{
	i32_t i = 0;

	if (p_ds->p_keys) {
		for (i=0; i<p_ds->key_size; i++) {
			if (p_ds->p_keys[i].val) {
				free(p_ds->p_keys[i].val);
				p_ds->p_keys[i].val = NULL;
			}
		}
		free(p_ds->p_keys);
		memset(p_ds, 0x00, sizeof(data_set_t));
	}

	return F1G_OK;
}
