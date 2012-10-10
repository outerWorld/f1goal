
#ifndef __F1G_DATA_OP_H
#define __F1G_DATA_OP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "f1g_basic_types.h"

//#define __DATA_ITEM_LIST

typedef struct _data_item_s {
#ifdef __DATA_ITEM_LIST
	struct _data_item_s * next;
#endif // __DATA_ITEM_LIST
	i32_t	size;
	i32_t	len;
	i8_p	data;
}data_item_t, *data_item_p;

typedef struct _data_field_s {
	i8_t		item_size;
	i8_t		item_num;
	data_item_p p_items;
}data_field_t, *data_field_p;

i8_t data_field_init(data_field_p p_obj, i8_t item_size, i32_t item_buf_size);

data_field_p data_field_create(i8_t item_size, i32_t item_buf_size);
/*
 * Set the buffer size of the specified item(pointed by seq), and the buffer is
 * created at the same time. Specially, the item_buf_size can be 0, if so, the
 * buffer for this item will be free if it exists, which will be used 
 */
i8_t data_field_set_item_size(data_field_p p_obj, i8_t seq, i32_t item_buf_size);
/*
 * clean history data, after this, data_field object can be used at next operation.
 */
i8_t data_field_clean_items(data_field_p p_obj);

/*
 * Free items.
 */
i8_t data_field_free_items(data_field_p p_obj);

i8_t data_field_read_buf(data_field_p p_obj, i8_p pbuf, i8_p pbuf_end, string_t sep);

i8_t data_field_read_str(data_field_p p_obj, string_t pstr, string_t sep);

i8_t data_field_add_item(data_field_p p_obj, i8_p pb, i8_p pe);
i8_t data_field_print(data_field_p p_obj);

const string_t data_field_get_str(data_field_p p_obj, i8_t seq);
i8_t data_field_copy_str(data_field_p p_obj, i8_t seq, i8_p buf, i32_t buf_size);
i32_t data_field_get_i32(data_field_p p_obj, i8_t seq, i8_t base);
u32_t data_field_get_u32(data_field_p p_obj, i8_t seq, i8_t base);
i64_t data_field_get_i64(data_field_p p_obj, i8_t seq, i8_t base);
u64_t data_field_get_u64(data_field_p p_obj, i8_t seq, i8_t base);

#ifdef __cplusplus
}
#endif

#endif // __F1G_DATA_OP_H
