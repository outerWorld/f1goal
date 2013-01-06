
#ifndef __F1G_QUEUE_H
#define __F1G_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"

typedef struct _elem_s {
	i32_t status; // free(0x00), in use(0x01), to be free(0x02)
	i32_t data_len;
}elem_t, *elem_p;

typedef i32_t pos_t;

typedef struct _que_obj_s {
	pos_t		front;
	pos_t		tail;
	i32_t		blk_size;
	i32_t		blk_num;
	i8_p		buffer;
}que_obj_t, *que_obj_p;


i8_t que_obj_init(que_obj_p p_que, i32_t blk_size, i32_t blk_num);

que_obj_p que_obj_create(i32_t blk_size, i32_t blk_num);

i32_t que_obj_blk_size(que_obj_p p_obj);

i32_t que_obj_blk_num(que_obj_p p_obj);

i8_t que_obj_empty(que_obj_p p_obj);
i8_t que_obj_full(que_obj_p p_obj);

i8_t que_obj_pop(que_obj_p p_obj);

i8_t que_obj_add(que_obj_p p_obj, i8_p p_data, i32_t data_size);

i8_p que_obj_head(que_obj_p p_obj);

i8_p que_obj_tail(que_obj_p p_obj);

i8_t que_obj_destroy(que_obj_p p_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
