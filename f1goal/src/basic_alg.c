/*
 * -*- encoding=utf-8 -*-
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "f1g_basic_funcs.h"
#include "basic_alg.h"

static int rotate_by_seq(char *arr, int arr_size, int left_k, extra_info_p p_extra)
{
	int i, j;
	int blk_size = p_extra->elem_size;
	char * temp = NULL;
	int next_pos = 0;

	MEM_ALOC(temp, char *, blk_size, -1);

	for (i=0; i<left_k; i++) {
		memcpy(temp, arr+0, blk_size);
		for (j=0; j<arr_size; j++) {
			next_pos = j * blk_size;
			memcpy(arr+next_pos, arr+next_pos+blk_size, blk_size);
		}
		memcpy(arr+(j-1)*blk_size, temp, blk_size);
	}
	
	MEM_FREE(temp);

	return 0;
}

int array_rotate(void *arr, int arr_size, int left_k, extra_info_p p_extra)
{
	left_k %= arr_size;
	switch (p_extra->method) {
		case METHOD_SEQ:
			return rotate_by_seq((char*)arr, arr_size, left_k, p_extra);
		case METHOD_3T:
			break;
		case METHOD_BLKSWP:
			break;

		case METHOD_GCD:
			break;
		default:
			return -1;
	}

	return 0;
}
