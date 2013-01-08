
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "f1g_queue.h"


#define BLK_ADDR(pos, p_obj) (p_obj->buffer + (pos) * p_obj->blk_size)

#define TAIL_BLK_ADDR(p_obj) (p_obj->buffer + p_obj->tail * p_obj->blk_size)
#define HEAD_BLK_ADDR(p_obj) (p_obj->buffer + p_obj->front * p_obj->blk_size)

i8_t que_obj_init(que_obj_p p_que, i32_t blk_size, i32_t blk_num)
{
	p_que->buffer = (i8_p)malloc((blk_size+sizeof(elem_t))*blk_num);
	if (!p_que->buffer) return F1G_ERR;
	
	p_que->blk_size = blk_size + sizeof(elem_t);
	p_que->blk_num = blk_num;
	p_que->tail = p_que->front = 0;

	return F1G_OK;
}

que_obj_p que_obj_create(i32_t blk_size, i32_t blk_num)
{
	que_obj_p p_obj = NULL;

	p_obj = (que_obj_p)malloc(sizeof(que_obj_t));
	if (!p_obj) return NULL;
	memset(p_obj, 0x00, sizeof(que_obj_t));	

	if (F1G_OK != que_obj_init(p_obj, blk_size, blk_num)) {
		free(p_obj);
		return NULL;
	}
	
	return p_obj;
}

i32_t que_obj_blk_size(que_obj_p p_obj)
{
	return p_obj->blk_size;
}

i32_t que_obj_blk_num(que_obj_p p_obj)
{
	return p_obj->blk_num;
}

i8_t que_obj_empty(que_obj_p p_obj)
{
	return (p_obj->front==p_obj->tail)?1:0;
}

i8_t que_obj_full(que_obj_p p_obj)
{
	return (p_obj->front==((p_obj->tail+1)%p_obj->blk_num))?1:0;
}

i8_t que_obj_pop(que_obj_p p_obj)
{
	elem_p p_elem = NULL;

	if (p_obj->front != p_obj->tail) {
		// not empty
		p_elem = (elem_p)BLK_ADDR(p_obj->front, p_obj);		
		p_elem->data_len = 0;
		p_elem->status = 0x00;
		p_obj->front = (p_obj->front+1)%(p_obj->blk_num);
	}

	return F1G_OK;
}

i8_t que_obj_add(que_obj_p p_obj, i8_p p_data, i32_t data_size)
{
	elem_p p_elem = NULL;
	
	if ((p_obj->blk_size-sizeof(elem_t)) < data_size) {
		return F1G_ERR;
	}

	if ((p_obj->tail+1)%(p_obj->blk_num) != p_obj->front) {
		// store data
		p_elem = (elem_p)TAIL_BLK_ADDR(p_obj);
		p_elem->data_len = data_size;
		p_elem->status = 0x01;
		memcpy(BLK_DATA((i8_p)p_elem), p_data, data_size);
		//
		p_obj->tail = (p_obj->tail + 1) % (p_obj->blk_num);
	}

	return F1G_OK;
}

i8_p que_obj_head(que_obj_p p_obj)
{
	if (!que_obj_empty(p_obj)) {
		return HEAD_BLK_ADDR(p_obj);
	} else {
		return NULL;
	}
}

i8_p que_obj_tail(que_obj_p p_obj)
{
	pos_t tail = p_obj->tail - 1;

	if (!que_obj_empty(p_obj)) {
		if (tail < 0) {
			tail = p_obj->blk_num - 1;
		}
		return BLK_ADDR(tail, p_obj);
	} else {
		return NULL;
	}
}

i8_p que_obj_next_freeblk(que_obj_p p_obj)
{
	return BLK_ADDR(p_obj->tail, p_obj);
}

i32_t que_obj_move_next(que_obj_p p_obj)
{
	if (que_obj_full(p_obj)) {
		return F1G_ERR;
	} else {
		p_obj->tail = (p_obj->tail + 1) % p_obj->blk_num;
		return F1G_OK;
	}

	return F1G_OK;
}

i8_t que_obj_destroy(que_obj_p p_obj)
{
	return F1G_OK;
}

i32_t que_obj_stat(que_obj_p p_obj)
{
	fprintf(stdout, "front=%d, tail=%d\n", p_obj->front, p_obj->tail);
	fprintf(stdout, "blk_size=%d, blk_num=%d\n", p_obj->blk_size, p_obj->blk_num);

	return F1G_OK;
}

