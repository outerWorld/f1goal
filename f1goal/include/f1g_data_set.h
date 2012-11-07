
#ifndef __F1G_DATA_SET_H
#define __F1G_DATA_SET_H

#include "f1g_basic_types.h"

typedef i32_t (*fmt_func_p)(void * p_key, string_t var_name, void *in_var, i32_t val_len, u8_t var_type);

#define MAX_KEY_LEN 	64
#define KEY_VAL_SIZE	128
typedef struct _key_info_s {
	i8_t		key[MAX_KEY_LEN+1];
	fmt_func_p 	p_func;
	value_t		val;
}key_info_t, *key_info_p;

i32_t key_info_extent_cap(key_info_p p_key, i32_t ext_size);

typedef struct _data_set_s {
	i32_t		key_num;
	i32_t		key_size;
	key_info_p 	p_keys;
}data_set_t, *data_set_p;

i32_t data_set_init(data_set_p p_ds, i32_t ini_key_size);

i32_t data_set_get_val(data_set_p p_ds, string_t key, value_p *val);
i32_t data_set_extent_cap(data_set_p p_ds, i32_t ext_key_size);

i32_t data_set_add_int_var(data_set_p p_ds, string_t var_name, void *in_var_val, string_t fmt);

i32_t data_set_add_str_var(data_set_p p_ds, string_t var_name, void *in_var_val, i32_t val_len);

i32_t data_set_destroy(data_set_p p_ds);

#endif // __F1G_DATA_SET_H
