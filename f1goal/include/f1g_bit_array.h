
#ifndef __F1G_BIT_ARRAY_H
#define __F1G_BIT_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"

typedef struct _bit_array_s {
	u8_t	bits_num;
	u8_p	bits;	
}bit_array_t, *bit_array_p;

i8_t bit_array_init(bit_array_p p_obj, u8_t bit_num);

/*
 * bits_num points out how many bits the user want
 */
bit_array_p bit_arrary_create(u8_t bits_num);

/*
 * bit_seq: start from 0
 * the return value could be:0,1 or -1(error occurs)
 */
i8_t bit_array_bit_value(bit_array_p p_obj, u8_t bit_seq);

i8_t bit_array_set_bit(bit_array_p p_obj, u8_t bit_seq);

i8_t bit_array_reset(bit_array_p p_obj);

i8_t bit_array_clean(bit_array_p p_obj);

void bit_array_destroy(bit_array_p p_bits);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_BIT_ARRAY_H
