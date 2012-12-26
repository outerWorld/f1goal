
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_server.h"


int main(int argc, char *argv[])
{
	server_conf_t serv_cfg;

	serv_load(argv[1], &serv_cfg);
	
	return 0;
}
