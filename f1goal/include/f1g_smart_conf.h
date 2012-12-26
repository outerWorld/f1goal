/*
 * Desc  : It provides a common used interface for reading configuration files of 
 *		   various format, and supports Multi-threads scene and Multi-processes scene.
 *
 * Author: huang chunping
 * Date  : 2012-11-27
 */

#ifndef __F1G_SMART_CONF_H
#define __F1G_SMART_CONF_H

#include "f1g_basic_types.h"

#define ITEM_NAME_SIZE	64
#define SEC_NAME_SIZE	64

typedef struct _item_s {
	i8_t	item_name[ITEM_NAME_SIZE];
	i32_t	val_len;
	i8_p	val;
}item_t, *item_p;

typedef struct _sec_s {
	i8_t	sec_name[SEC_NAME_SIZE];
	i32_t	item_num;
	i32_t	item_size;
	item_p  p_items;
}sec_t, *sec_p;

// supported configuration file types.
// 1 format of CFT_INI:
//	[section]
//		item1 = data1
//		item2 = data2
// 2 format of CFT_LINE:
//  line1: "item1" "seperator" "item2" "seperator" "item3"
//  line2: "item1" "seperator" "item2" "seperator" "item3"
// 	so, section is replaced by line number. 
enum {
	CFT_INI = 0x00,
	CFT_LINE,
};

typedef struct _smart_conf_s {
	i32_t	sec_num;
	i32_t	sec_size;
	sec_p	p_secs;
}smart_conf_t, *smart_conf_p;

i32_t smart_conf_init(smart_conf_p p_conf, i32_t sec_size, i32_t item_size);

i32_t smart_conf_read(smart_conf_p p_conf, string_t conf_file, i32_t conf_type);

i32_t smart_conf_dbg(smart_conf_p p_conf);

i32_t smart_conf_get_u32(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u32_t dft_val, u32_p u32_val);
i32_t smart_conf_get_x32(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u32_t dft_val, u32_p u32_val);

i32_t smart_conf_get_i32(smart_conf_p p_conf, string_t sec, string_t item_name,
												 i32_t dft_val, i32_p i32_val);

i32_t smart_conf_get_i64(smart_conf_p p_conf, string_t sec, string_t item_name,
												 i64_t dft_val, i64_p i64_val);

i32_t smart_conf_get_u64(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u64_t dft_val, u64_p u64_val);

i32_t smart_conf_get_x64(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u64_t dft_val, u64_p u64_val);

i32_t smart_conf_get_str(smart_conf_p p_conf, string_t sec, string_t item_name,
								 string_t dft_val, i8_p p_buf, i32_t buf_size);

i32_t smart_conf_destroy(smart_conf_p p_conf);

#endif // __F1G_SMART_CONF_H
