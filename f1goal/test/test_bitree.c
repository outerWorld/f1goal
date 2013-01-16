
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bitree.h"

static int test_arr[10] = { 0 };

int main(int argc, char *argv[])
{
	int i;
	bitree_p p_bt = NULL;

	p_bt = bitree_create(8);
	for (i=0; i<10; i++) {
		p_bt->height += bitree_insert(p_bt, i);
	}
	
	fprintf(stdout, "bitree_prev_show\n");
	bitree_prev_show(p_bt);
	fprintf(stdout, "bitree_mid_show\n");
	bitree_mid_show(p_bt);
	fprintf(stdout, "bitree_post_show\n");
	bitree_post_show(p_bt);

	return 0;
}
