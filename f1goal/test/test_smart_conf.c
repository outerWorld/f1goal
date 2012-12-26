
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_smart_conf.h"

int main(int argc, char *argv[])
{
	smart_conf_t conf;
	i32_t i32;
	u32_t u32;
	i64_t i64;
	u64_t u64;
	i8_t  buf[1024];
	
	
	if (smart_conf_init(&conf, 2, 2) != F1G_OK) {
		fprintf(stderr, "smart_conf_init fail!\n");
		return -1;
	}

	if (smart_conf_read(&conf, argv[1], CFT_INI) != F1G_OK) {
		fprintf(stderr, "smart_conf_read fail!\n");
		return -1;
	}

	smart_conf_dbg(&conf);

	smart_conf_get_str(&conf, "address", "ip", "127.0.0.1", buf, 1024);
	fprintf(stdout, "address,ip = %s\n", buf);
	smart_conf_get_i32(&conf, "address", "port", 10007, &i32);
	fprintf(stdout, "address, port = %d\n", i32);
	
	smart_conf_get_str(&conf, "node", "node_type", "umc", buf, 1024);
	fprintf(stdout, "node, node_type = %s\n", buf);
	smart_conf_get_str(&conf, "node", "node_name", "bj", buf, 1024);
	fprintf(stdout, "node, node_name = [%s]\n", buf);

	if (F1G_OK != smart_conf_get_i32(&conf, "node", "node_id", 12, &i32)) {
		fprintf(stderr, "get node node_id failed\n");
	}
	fprintf(stdout, "node,node_id = %d\n", i32);
	
	smart_conf_destroy(&conf);

	return 0;
}
