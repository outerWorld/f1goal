
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_url_parser.h"

/*
 * Usage: test_url_parser url
 */
int main(int argc, char *argv[])
{
	url_parser_t url_parser;

	if (argc < 2) return 1;

	url_parser_init(&url_parser, 0);
	url_parser_load(&url_parser, argv[1], strlen(argv[1]));
	url_parser_show(&url_parser);
	url_parser_clear(&url_parser);

	url_parser_destroy(&url_parser);

	return 0;
}
