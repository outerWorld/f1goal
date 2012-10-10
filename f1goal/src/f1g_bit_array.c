
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_bit_array.h"

i8_t bit_array_init(bit_array_p p_obj, u8_t bit_num)
{
	u32_t 		byte_num;
	byte_num = bit_num/8;
	if (bit_num%8 != 0) {
		byte_num += 1;
	}

	p_obj->bits = (u8_p)malloc(byte_num);
	if (NULL == p_obj->bits) {
		return F1G_ERR;
	}
	memset(p_obj->bits, 0x00, byte_num);

	p_obj->bits_num = bit_num;

	return F1G_OK;
}

bit_array_p bit_arrary_create(u8_t bit_num)
{
	bit_array_p p_obj;
	
	p_obj = (bit_array_p)malloc(sizeof(bit_array_t));	
	if (NULL == p_obj) {
		return NULL;
	}
	memset(p_obj, 0x00, sizeof(bit_array_t));

	if (F1G_OK != bit_array_init(p_obj, bit_num) ) {
		free(p_obj);
		p_obj = NULL;
		return NULL;
	}

	return p_obj;
}

i8_t bit_array_bit_value(bit_array_p p_obj, u8_t bit_seq)
{
	if (bit_seq >= p_obj->bits_num) {
		return -1;
	}
	
	return ((p_obj->bits[(bit_seq>>3)])<<(7-bit_seq%8))>>7;
}

i8_t bit_array_set_bit(bit_array_p p_obj, u8_t bit_seq)
{
	if (bit_seq >= p_obj->bits_num) {
		return F1G_ERR;
	}

	p_obj->bits[(bit_seq>>3)] |= 0x01<<(bit_seq%8);
	
	return F1G_OK;
}

i8_t bit_array_reset(bit_array_p p_obj)
{
	if (NULL == p_obj->bits) {
		return F1G_ERR;
	}

	u8_t bytes = p_obj->bits_num/8;
	if (p_obj->bits_num%8 != 0) {
		bytes += 1;
	}
	memset(p_obj->bits, 0x00, bytes);

	return F1G_OK;
}

i8_t bit_array_clean(bit_array_p p_obj)
{
	if (NULL != p_obj->bits) {
		free(p_obj->bits);
	}
	return F1G_OK;
}

void bit_array_destroy(bit_array_p p_obj)
{
	if (NULL == p_obj) {
		return;
	}

	bit_array_clean(p_obj);

	free(p_obj);
}
