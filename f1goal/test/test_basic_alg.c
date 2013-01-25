// -*- encoding=utf-8 -*-

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "basic_alg.h"

typedef struct _chunk_s {
	int 	id;
	int 	size;
}chunk_t, *chunk_p;

static void show_chunk(chunk_p p_chunk, int size)
{
	int i;

	for (i=0; i<size; i++) {
		if ((i+1) % 16 == 0) fprintf(stdout, "%d,%d\n", p_chunk[i].id, p_chunk[i].size);
		else fprintf(stdout, "%d,%d ",p_chunk[i].id, p_chunk[i].size);
	}

	fprintf(stdout, "\n");
}

static void show_int_array(int *arr, int size)
{
	int i;

	for (i=0; i<size; i++) {
		if ((i+1) % 16 == 0) fprintf(stdout, "%d\n", arr[i]);
		else fprintf(stdout, "%d ", arr[i]);
	}
	fprintf(stdout, "\n");
}

static void show_string_array(char *arr[], int size)
{
	int i;

	for (i=0; i<size; i++) {
		if ((i+1) % 16 == 0) fprintf(stdout, "%s\n", arr[i]);
		else fprintf(stdout, "%s ", arr[i]);
	}
	fprintf(stdout, "\n");
}

static char *test_str_arr[] = {
	"a", "b", "c", "d", "e",
	"f", "h", "i", "j", "k",
	NULL	
};


int main(int argc, char *argv[])
{
	int i;
	int arr[10];
	char *str_arr[10];
	extra_info_t extra;
	chunk_t chunks[10];

	fprintf(stdout, "sizeof(void*)=%d, sizeof(char*)=%d\n", sizeof(void*), sizeof(char*));

	for (i=0; i<10; i++) arr[i] = i+1;
	show_int_array(arr, 10);

	extra.elem_size = sizeof(int);
	extra.method = METHOD_SEQ;
	extra.elem_type = 0;

	array_rotate((void*)arr, 10, 3, &extra);
	show_int_array(arr, 10);

	for (i=0; i<10; i++) str_arr[i] = test_str_arr[i];
	show_string_array(str_arr, 10);

	extra.elem_size = sizeof(char*);
	extra.method = METHOD_SEQ;
	extra.elem_type = 0;
	array_rotate((void*)str_arr, 10, 3, &extra);
	show_string_array(str_arr, 10);

	for (i=0; i<10; i++) { chunks[i].id = i; chunks[i].size = i; }
	show_chunk(chunks, 10);
	extra.elem_size = sizeof(chunk_t);
	extra.method = METHOD_SEQ;
	extra.elem_type = 0;
	array_rotate((void*)chunks, 10, 3, &extra);
	show_chunk(chunks, 10);

	return 0;
}
