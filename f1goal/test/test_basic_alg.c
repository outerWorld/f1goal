// -*- encoding=utf-8 -*-

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "basic_alg.h"

static void show_int_array(int *arr, int size)
{
	int i;

	for (i=0; i<size; i++) {
		if ((i+1) % 16 == 0) fprintf(stdout, "%d\n", arr[i]);
		else fprintf(stdout, "%d ", arr[i]);
	}
	fprintf(stdout, "\n");
}

static void show_string_array(char **arr, int size)
{
	int i;

	for (i=0; i<size; i++) {
		if ((i+1) % 16 == 0) fprintf(stdout, "%s\n", arr[i]);
		else fprintf(stdout, "%s ", arr[i]);
	}
	fprintf(stdout, "\n");
}

int main(int argc, char *argv[])
{
	int i;
	int arr[10];
	char str_arr[10][10];
	extra_info_t extra;
	fprintf(stdout, "sizeof(void*)=%d, sizeof(char*)=%d\n", sizeof(void*), sizeof(char*));

	for (i=0; i<10; i++) arr[i] = i+1;
	show_int_array(arr, 10);

	extra.elem_size = sizeof(int);
	extra.method = METHOD_SEQ;
	extra.elem_type = 0;

	array_rotate((void*)arr, 10, 3, &extra);
	show_int_array(arr, 10);

	for (i=0; i<10; i++) sprintf(str_arr[i], "%d", i);
	show_string_array(str_arr, 10);

	extra.elem_size = 10;
	extra.method = METHOD_SEQ;
	extra.elem_type = 0;
	array_rotate((void*)str_arr, 10, 3, &extra);
	show_string_array(str_arr, 10);

	return 0;
}
