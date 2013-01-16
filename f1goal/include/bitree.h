/*
 * Author: huang chunping
 * Date  : 2013-01-16
 */

#ifndef __BITREE_H
#define __BITREE_H
struct _bitree_s;
typedef struct _bitree_s bitree_t;
typedef bitree_t * bitree_p;
typedef bitree_p bitree_node_p;

struct _bitree_s {
	int elem;
	int height;
	bitree_p p_left, p_right;
};

bitree_p bitree_create(int elem);

int bitree_height(bitree_p p_bt);

int bitree_insert(bitree_p p_bt, int elem);

int bitree_del(bitree_p p_bt, int elem);

int bitree_avl_insert(bitree_p p_bt, int elem);

int bitree_avl_del(bitree_p p_bt, int elem);

int bitree_post_show(bitree_p p_bt);

int bitree_prev_show(bitree_p p_bt);

int bitree_mid_show(bitree_p p_bt);

int bitree_empty(bitree_p p_bt);

int bitree_destroy(bitree_p p_bt);

#endif // __BITREE_H
