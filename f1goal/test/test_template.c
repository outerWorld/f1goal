
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_template.h"

int main(int argc, char *argv[])
{
	char *file = NULL;
	f1g_template_t tpl;

	if (argc < 2) {
		return 1;
	}
	file = argv[1];

	if (F1G_OK != f1g_template_init(&tpl, 24)) {
		fprintf(stderr, "Init template fail!\n");
		return 1;
	}
	
	if (F1G_OK != f1g_template_parse(&tpl, file)) {
		fprintf(stderr, "Parse file %s fail!\n", file);
		return 1;
	}
	
	f1g_template_print(&tpl);

	f1g_template_destroy(&tpl);
	return 0;
}
