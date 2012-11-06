
#ifndef __F1G_QUEUE_H
#define __F1G_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"

typedef struct _queue_object_s {
	i32_t		front;
	i32_t		tail;
	i32_t		blk_size;
	i32_t		blk_num;
	i8_p		buffer;
}queue_object_t, *queue_object_p;

i8_t queue_object_init(queue_object_p p_que, i32_t blk_size, i32_t blk_num);

queue_object_p queue_object_create(i32_t blk_size, i32_t blk_num);

i32_t queue_object_blk_size(queue_object_p p_obj);

i32_t queue_object_blk_num(queue_object_p p_obj);

i8_t queue_object_pop(queue_object_p p_obj);

i8_t queue_object_add(queue_object_p p_obj, i8_p p_data, i32_t data_size);

i8_p queue_object_head(queue_object_p p_obj);

i8_p queue_object_tail(queue_object_p p_obj);

i8_t queue_object_destroy(queue_object_p p_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
