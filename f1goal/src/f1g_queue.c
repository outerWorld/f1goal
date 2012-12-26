
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "f1g_queue.h"

i8_t queue_object_init(queue_object_p p_que, i32_t blk_size, i32_t blk_num)
{
	return F1G_OK;
}

queue_object_p queue_object_create(i32_t blk_size, i32_t blk_num)
{
	return F1G_OK;
}

i32_t queue_object_blk_size(queue_object_p p_obj)
{
	return F1G_OK;
}

i32_t queue_object_blk_num(queue_object_p p_obj)
{
	return F1G_OK;
}

i8_t queue_object_pop(queue_object_p p_obj)
{
	return F1G_OK;
}

i8_t queue_object_add(queue_object_p p_obj, i8_p p_data, i32_t data_size)
{
	return F1G_OK;
}

i8_p queue_object_head(queue_object_p p_obj)
{
	return F1G_OK;
}

i8_p queue_object_tail(queue_object_p p_obj)
{
	return F1G_OK;
}

i8_t queue_object_destroy(queue_object_p p_obj)
{
	return F1G_OK;
}
