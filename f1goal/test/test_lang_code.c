
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_basic_funcs.h"
#include "f1g_lang_code.h"

// Usage: ./test_lang_code input_string
int main(int argc, char *argv[])
{
	int ret;
	char buf[1024] = { 0 };
	int data_len = 0;
	unsigned char code_type = LANG_CODE_UNKNOWN;

	if (argc < 2) {
		fprintf(stderr, "Wrong! Not Enough Arguments!\n");
		return 1;
	}

	lang_code_init();
	
	data_len = 1024;
	ret = str_decode(argv[1], strlen(argv[1]), buf, &data_len, &code_type);
	if (F1G_OK != ret) {
		fprintf(stderr, "[%s,%d] str_decode fail!\n", __FUNCTION__, __LINE__);
		return 1;
	}
	hex_print(stdout, buf, data_len);

	return 0;
}
