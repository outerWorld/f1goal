/*
 * -*- encoding=utf-8 -*-
 * Desc  : Basic alogrithms
 * Author: huang chunping
 * Date  : 2013-01-24
 */

#ifndef __BASIC_ALG_H
#define __BASIC_ALG_H

// definintion of rotate method
enum {
	METHOD_SEQ = 0x00, // the simplest basic method
	METHOD_3T, 			// XY --> (X^TY^T)^T
	METHOD_BLKSWP, 		// ab -->a0a1b -->ba1a0 --> .....-->ba0a1
	METHOD_GCD, 		// gcd (greatest common divisor)
	METHOD_NUM
};

typedef struct _extra_info_s {
	int 	method;		// process method
	int 	elem_size;  // byte size of array element.
	int		elem_type;	// type of array element, system defined as int or char * or user defined
}extra_info_t, *extra_info_p;

/*
 * 来源：左旋字符串，如[a,b,c,d,e]，左旋3的结果为[d,e,a,b,c]
 * 
 */
int array_rotate(void *arr, int arr_size, int left_k, extra_info_p p_extra);

#endif
