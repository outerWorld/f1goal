
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_basic_funcs.h"
#include "f1g_data_op.h"

i8_t data_field_init(data_field_p p_obj, i8_t item_size, i32_t item_buf_size)
{
	i32_t i;

	p_obj->p_items = (data_item_p)malloc(sizeof(data_item_t)*item_size);
	if (NULL == p_obj->p_items) {
		return F1G_ERR;
	}
	memset(p_obj->p_items, 0x00, sizeof(data_item_t)*item_size);
	
	p_obj->item_num = 0;
	p_obj->item_size = item_size;
	if (item_buf_size <= 0) {
		return F1G_OK;	
	}

	for (i=0; i<item_size; i++) {
		p_obj->p_items[i].data = (i8_p)malloc(item_buf_size);

		if (NULL == p_obj->p_items[i].data) {
			while (--i >= 0) {
				if (NULL != p_obj->p_items[i].data) {
					free(p_obj->p_items[i].data);
					p_obj->p_items[i].data = NULL;
				}
			}
			free (p_obj->p_items);
			p_obj->p_items = NULL;
			return F1G_ERR;
		}

		p_obj->p_items[i].size = item_buf_size;
	}


	return F1G_OK;
}

data_field_p data_field_create(i8_t item_size, i32_t item_buf_size)
{
	data_field_p p_obj;

	if (item_size <= 0) {
		return NULL;
	}

	p_obj = (data_field_p)malloc(sizeof(data_field_t));
	if (NULL == p_obj) {
		return NULL;
	}
	memset(p_obj, 0x00, sizeof(data_field_t));

	if (F1G_OK != data_field_init(p_obj, item_size, item_buf_size) ) {
		free(p_obj);
		p_obj = NULL;
		return NULL;
	}

	return p_obj;
}

i8_t data_field_set_item_size(data_field_p p_obj, i8_t seq, i32_t item_buf_size)
{
	if (seq >= p_obj->item_size) {
		return F1G_ERR;
	}

	if (item_buf_size <= 0) {
		free(p_obj->p_items[seq].data);
		p_obj->p_items[seq].data = NULL;
		p_obj->p_items[seq].size = 0;
		p_obj->p_items[seq].len = 0;
	} else {
		free(p_obj->p_items[seq].data);
		memset(&p_obj->p_items[seq], 0x00, sizeof(data_item_t));

		p_obj->p_items[seq].data = (i8_p)malloc(item_buf_size);
		if (NULL == p_obj->p_items[seq].data) {
			return F1G_ERR;
		}
		memset(p_obj->p_items[seq].data, 0x00, item_buf_size);
		p_obj->p_items[seq].size = item_buf_size;
		p_obj->p_items[seq].len = 0;
	}

	return F1G_OK;
}

i8_t data_field_clean_items(data_field_p p_obj)
{
	i32_t	i;

	for (i=0; i<p_obj->item_num; i++) {
		p_obj->p_items[i].len = 0;
		if (NULL != p_obj->p_items[i].data) {
			p_obj->p_items[i].data[0] = '\0';
		}
	}

	p_obj->item_num = 0;

	return F1G_OK;
}

i8_t data_field_free_items(data_field_p p_obj)
{
	i32_t i;
	
	for (i=0; i<p_obj->item_num; i++) {
		if (NULL != p_obj->p_items[i].data) {
			free(p_obj->p_items[i].data);
		}
	}
	memset(p_obj->p_items, 0x00, sizeof(data_item_t)*p_obj->item_num);
	free(p_obj->p_items);
	p_obj->p_items = NULL;
	p_obj->item_size = 0;
	p_obj->item_num = 0;

	return F1G_OK;
}

i8_t data_field_add_item(data_field_p p_obj, i8_p pb, i8_p pe)
{
	i8_t seq;
	data_item_p ptmp;
	i32_t new_item_size;
	

	// make sure there is enough capcity for new items;
	if (p_obj->item_num >= p_obj->item_size) {
		ptmp = p_obj->p_items;
		new_item_size = p_obj->item_size + 4;
		p_obj->p_items = (data_item_p)malloc(new_item_size*sizeof(data_item_t));
		if (NULL == p_obj->p_items) {
			p_obj->p_items = ptmp;
			return F1G_ERR;
		}
		memcpy(p_obj->p_items, ptmp, p_obj->item_size*sizeof(data_item_t));
		p_obj->item_size = new_item_size;
	}
	
	seq = p_obj->item_num;
	if (p_obj->p_items[seq].size <= (pe-pb+1)) {
		if (F1G_OK != data_field_set_item_size(p_obj, seq, pe-pb+1+1)) {
			return F1G_ERR;
		}
	}
	memcpy(p_obj->p_items[seq].data, pb, pe-pb);
	p_obj->p_items[seq].data[pe-pb] = '\0';
	p_obj->p_items[seq].len = pe-pb;
	p_obj->item_num++;
	
	return F1G_OK;
}

i8_t data_field_read_buf(data_field_p p_obj, i8_p pbuf, i8_p pbuf_end, string_t sep)
{
	i8_p pb;
	i8_p pe;
	i32_t sep_len;

	sep_len = strlen(sep);
	pb = pbuf;
	pe = NULL;
	while (pb != pbuf_end) {
		find_str(pb, pbuf_end, sep, &pe);

		if (F1G_OK != data_field_add_item(p_obj, pb, pe) ) {
			return F1G_ERR;
		}
		
		if (pe == pbuf_end) {
			pb = pbuf_end;
		} else {
			pb = pe + sep_len;
			pe = NULL;
		}
	}

	return F1G_OK;
}

i8_t data_field_read_str(data_field_p p_obj, string_t pstr, string_t sep)
{
	return data_field_read_buf(p_obj, pstr, pstr+strlen(pstr), sep);
}

i8_t data_field_print(data_field_p p_obj)
{
	i8_t i;
	
	for (i=0; i<p_obj->item_num; i++) {
		if (NULL != p_obj->p_items[i].data) {
			fprintf(stdout, "[Field %d][%.*s]\n", i, p_obj->p_items[i].len, p_obj->p_items[i].data);
		}
	}

	return F1G_OK;
}

const string_t data_field_get_str(data_field_p p_obj, i8_t seq)
{
	if (seq >= p_obj->item_num) {
		return NULL;
	}
	
	return p_obj->p_items[seq].data;
}

i8_t data_field_copy_str(data_field_p p_obj, i8_t seq, i8_p buf, i32_t buf_size)
{
	data_item_p p_item;
	i32_t len;

	if (seq >= p_obj->item_num) {
		return F1G_ERR;
	}
	p_item = &p_obj->p_items[seq];

	if (buf_size < p_item->len) {
		len = buf_size;
	} else {
		len = p_item->len;
	}
	memcpy(buf, p_item->data, len);
	buf[len] = '\0';

	return F1G_OK;
}

i32_t data_field_get_i32(data_field_p p_obj, i8_t seq, i8_t base)
{
	i32_t value;

	if (seq >= p_obj->item_num) {
		return -1;
	}

	value = strtol(p_obj->p_items[seq].data, NULL, base);

	return value;
}

u32_t data_field_get_u32(data_field_p p_obj, i8_t seq, i8_t base)
{
	return (u32_t)data_field_get_i32(p_obj, seq, base);
}

i64_t data_field_get_i64(data_field_p p_obj, i8_t seq, i8_t base)
{
	i64_t value;

	if (seq >= p_obj->item_num) {
		return -1;
	}

	value = strtoll(p_obj->p_items[seq].data, NULL, base);

	return value;
}

u64_t data_field_get_u64(data_field_p p_obj, i8_t seq, i8_t base)
{
	return (u64_t)data_field_get_i64(p_obj, seq, base);
}
