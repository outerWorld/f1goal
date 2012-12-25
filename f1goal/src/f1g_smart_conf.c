
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_smart_conf.h"


#define DFT_SEC_SIZE	20
#define DFT_ITEM_SIZE	20
i32_t smart_conf_init(smart_conf_p p_conf, i32_t sec_size, i32_t item_size)
{
	i32_t i, j;
	sec_p p_sec;
	item_p p_item;

	if (sec_size <= 0) { sec_size = DFT_SEC_SIZE; }
	if (item_size <= 0) { item_size = DFT_ITEM_SIZE; }

	p_sec = (sec_p)malloc(sizeof(sec_t)*sec_size);
	if (!p_sec) { return F1G_ERR; }
	memset(p_sec, 0x00, sizeof(sec_t)*sec_size);

	for (i=0; i<sec_size; i++) {
		p_item = (item_p)malloc(item_size*sizeof(item_t));
		if (!p_item) { return F1G_ERR; }
		memset(p_item, 0x00, item_size*sizeof(item_t));
		p_sec[i].p_items = p_item;
		p_sec[i].item_num = 0;
		p_sec[i].item_size = item_size;
	}

	p_conf->sec_num = 0;
	p_conf->sec_size = sec_size;
	p_conf->p_secs = p_sec;

	return F1G_OK;
}

static i32_t extend_sec_size(smart_conf_p p_conf, int ext_sec_size)
{
	sec_p p_sec = p_conf->p_secs;

	p_conf->p_secs = (sec_p)malloc(sizeof(sec_t)*ext_sec_size);
	if (!p_conf->p_secs) { return F1G_ERR; }
	memset(p_conf->p_secs, 0x00, sizeof(sec_t)*ext_sec_size);

	if (p_conf->sec_num > 0) { memcpy(p_conf->p_secs, p_sec, p_conf->sec_num*sizeof(sec_t)); }
	
	p_conf->sec_size = ext_sec_size;

	if (p_sec) { free(p_sec); }
	
	return F1G_OK;
}

static i32_t extend_item_size(sec_p p_sec, int ext_item_size)
{
	item_p p_item = p_sec->p_items;

	p_sec->p_items = (item_p)malloc(sizeof(item_t)*ext_item_size);
	if (!p_sec->p_items) { return F1G_ERR; }
	memset(p_sec->p_items, 0x00, sizeof(item_t)*ext_item_size);

	if (p_sec->item_num > 0) { memcpy(p_sec->p_items, p_item, p_sec->item_num*sizeof(item_t)); }
	
	p_sec->item_size = ext_item_size;

	if (p_item) { free(p_item); }
	
	return F1G_OK;
}


static i32_t get_line(i8_p p_data, i32_t data_len, i8_p p_line, i32_t line_size)
{
	return F1G_OK;
}

/*
 * support: "#" as comment
 */
static i32_t ini_read(smart_conf_p p_conf, string_t conf_file) {
	i32_t fd = 0;
	i8_p buf = NULL;
	i32_t buf_size = 0;
	i8_p cur_sec = NULL;
	i8_p cur_item = NULL;

	fd = open(conf_file, O_RDONLY);
	if (fd <= 0) { return F1G_ERR; }
	
	while (1) {
	}
	
	return F1G_OK;
}

i32_t smart_conf_read(smart_conf_p p_conf, string_t conf_file, i32_t conf_type)
{
	switch (conf_type) {
		case CFT_INI:
			return ini_read(p_conf, conf_file);
			break;
		case CFT_LINE:
			// not support now.
			break;
		default:
			break;
	}

	return F1G_OK;
}

i32_t smart_conf_get_u32(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u32_t dft_val, u32_p u32_val)
{
}

i32_t smart_conf_get_i32(smart_conf_p p_conf, string_t sec, string_t item_name,
												 i32_t dft_val, i32_p i32_val)
{
}

i32_t smart_conf_get_i64(smart_conf_p p_conf, string_t sec, string_t item_name,
												 i64_t dft_val, i64_p i64_val)
{
}


i32_t smart_conf_get_u64(smart_conf_p p_conf, string_t sec, string_t item_name,
												 u64_t dft_val, u64_p u64_val)
{
}

i32_t smart_conf_get_str(smart_conf_p p_conf, string_t sec, string_t item_name,
								 string_t dft_val, i8_p p_buf, i32_t buf_size)
{
}

i32_t smart_conf_destroy(smart_conf_p p_conf)
{
}

