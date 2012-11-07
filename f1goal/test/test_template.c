
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "f1g_basic_funcs.h"
#include "f1g_template.h"

static char sg_buf[2048] = { 0 };

int main(int argc, char *argv[])
{
	int ret;
	int i_val = 0;
	long long ll_val = 0;
	struct timeval tvb, tve;
	char tmp_buf[256] = { 0 };

	char *file = NULL;
	f1g_template_t tpl;
	data_set_t ds;
	value_t buf;

	buf.val = (i8_p)sg_buf;
	buf.val_len = 0;
	buf.val_size = 2048;

	if (argc < 2) {
		return 1;
	}
	file = argv[1];

	gettimeofday(&tvb, NULL);
	if (F1G_OK != f1g_template_init(&tpl, 24)) {
		fprintf(stderr, "Init template fail!\n");
		return 1;
	}
	
	if (F1G_OK != f1g_template_parse(&tpl, file)) {
		fprintf(stderr, "Parse file %s fail!\n", file);
		return 1;
	}
	gettimeofday(&tve, NULL);
	fprintf(stdout, "Init Template consumes %d useconds\n", time_interval(&tvb, &tve));
	
	f1g_template_print(&tpl);

	gettimeofday(&tvb, NULL);
	ret = data_set_init(&ds, 10);
	if (F1G_OK != ret) {
		return 1;
	}
	
	i_val = 0x0a000222;
	ll_val = 0x1234567801020304;
	data_set_add_int_var(&ds, "iP", (void*)&i_val, "%08x");
	data_set_add_str_var(&ds, "ADc_ip", "1.1.1.1", 7);
	sprintf(tmp_buf, "%s", "www.google.com.hk/test.html");
	data_set_add_str_var(&ds, "O_URL", tmp_buf, strlen(tmp_buf));
	data_set_add_int_var(&ds, "UMASK", (void*)&ll_val, "%llu");
	i_val = 300;
	data_set_add_int_var(&ds, "INTERVAL", (void*)&i_val, "%d");
	i_val = 0x802040;
	data_set_add_int_var(&ds, "VASK", (void*)&i_val, "%u");
	i_val = 12300123;
	data_set_add_int_var(&ds, "UiD", (void*)&i_val, "%u");
	i_val = 0x00007705;
	data_set_add_int_var(&ds, "ANAME", (void*)&i_val, "%08x");
	i_val = 0;
	data_set_add_int_var(&ds, "IPC_NID", (void*)&i_val, "%1d");
	
	if (f1g_template_exec(&tpl, &buf, &ds) != F1G_OK) {
		f1g_template_print(&tpl);
		return 1;
	}
	gettimeofday(&tve, NULL);
	fprintf(stdout, "Exec Template consumes %d useconds\n", time_interval(&tvb, &tve));

	fprintf(stdout, "\nBuff[%d][%.*s]\n", buf.val_len, buf.val_len, buf.val);

	data_set_destroy(&ds);

	f1g_template_destroy(&tpl);

	return 0;
}
