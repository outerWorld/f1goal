
#ifndef __F1G_INI_CONF_H
#define __F1G_INI_CONF_H

#include "f1g_basic_types.h"
#include "f1g_string.h"

typedef struct _ini_item_s {
	v_string_t item_name;	
	v_string_t item_value;
}ini_item_t, *ini_item_p;

typedef struct _ini_sec_s {
	i32_t 		item_num;
	i32_t		item_size;
	ini_item_p 	p_items;
}ini_sec_t, *ini_sec_p;

typedef struct _ini_conf_s {
	i32_t		sec_num;
	i32_t		sec_size;
	ini_sec_p	p_secs;
}ini_conf_t, *ini_conf_p;

ini_conf_p ini_config_produce(string_t conf_name);

i32_t ini_config_free(ini_conf_p p_conf);

#endif // __F1G_INI_CONF_H
