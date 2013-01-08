
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

#define BLK_DATA(buffer) (buffer + sizeof(elem_t))

#define BLK_DATA_LEN(buffer) (((elem_p)(buffer))->data_len)

#define BLK_SIZE(p_obj) (p_obj->blk_size - sizeof(elem_t))

i8_t que_obj_init(que_obj_p p_que, i32_t blk_size, i32_t blk_num);

que_obj_p que_obj_create(i32_t blk_size, i32_t blk_num);

i32_t que_obj_blk_size(que_obj_p p_obj);

i32_t que_obj_blk_num(que_obj_p p_obj);

i8_t que_obj_empty(que_obj_p p_obj);

i8_t que_obj_full(que_obj_p p_obj);

// delete the head of queue and move to the next.
i8_t que_obj_pop(que_obj_p p_obj);

// add data at tail of queue
i8_t que_obj_add(que_obj_p p_obj, i8_p p_data, i32_t data_size);

// get the valid data at head of queue
i8_p que_obj_head(que_obj_p p_obj);

// get the valid data at tail of queue 
i8_p que_obj_tail(que_obj_p p_obj);

// get next free block at tail of queue, it will reduce the COPY operation
i8_p que_obj_next_freeblk(que_obj_p p_obj);
// move the tail to the next position, it is called after que_obj_next_freeblk called to add one valid data
i32_t que_obj_move_next(que_obj_p p_obj);

i8_t que_obj_destroy(que_obj_p p_obj);

// output the status of the queue
i32_t que_obj_stat(que_obj_p p_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
