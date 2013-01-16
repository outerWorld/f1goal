
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bitree.h"

bitree_p bitree_create(int elem)
{
	bitree_p p_bi;

	p_bi = (bitree_p)malloc(sizeof(bitree_t));
	if (!p_bi) return NULL;
	memset(p_bi, 0x00, sizeof(bitree_t));
	p_bi->elem = elem;
	p_bi->height = 0;

	return p_bi;
}

int bitree_insert(bitree_p p_bt, int elem)
{
	bitree_node_p * p_node;
	int height_added = 0;

	if (elem < p_bt->elem) {
		if (!p_bt->p_left) {
			p_bt->p_left = bitree_create(elem);
			if (!p_bt->p_right) height_added = 1;
		} else {
			p_bt->p_left->height += bitree_insert(p_bt->p_left, elem);
		}
	} else if (elem > p_bt->elem) {
		if (!p_bt->p_right) {
			p_bt->p_right = bitree_create(elem);
			if (!p_bt->p_left) height_added = 1;
		} else {
			p_bt->p_right->height += bitree_insert(p_bt->p_right, elem);
		}
	} else {
	}

	return height_added;
}

int bitree_del(bitree_p p_bt, int elem)
{
}

int bitree_avl_insert(bitree_p p_bt, int elem)
{
}

int bitree_avl_del(bitree_p p_bt, int elem)
{
}

int bitree_post_show(bitree_p p_bt)
{
	if (p_bt) {
		bitree_post_show(p_bt->p_left);
		bitree_post_show(p_bt->p_right);
		fprintf(stdout, "%.*s %d [height=%d]\n", p_bt->height, " ", p_bt->elem, p_bt->height);
	}

	return 0;
}

int bitree_prev_show(bitree_p p_bt)
{
	if (p_bt) {
		fprintf(stdout, "%.*s %d [height=%d]\n", p_bt->height, " ", p_bt->elem, p_bt->height);
		bitree_prev_show(p_bt->p_left);
		bitree_prev_show(p_bt->p_right);
	}
	
	return 0;
}

int bitree_mid_show(bitree_p p_bt)
{
	if (p_bt) {
		bitree_mid_show(p_bt->p_left);
		fprintf(stdout, "%.*s %d [height=%d]\n", p_bt->height, " ", p_bt->elem, p_bt->height);
		bitree_mid_show(p_bt->p_right);
	}
	
	return 0;
}

int bitree_empty(bitree_p p_bt)
{
}

int bitree_destroy(bitree_p p_bt)
{
}
