
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_basic_types.h"
#include "f1g_basic_funcs.h"
#include "f1g_cmd_line.h"

#include "pksender.h"

/*
 * Usage : pksender -p packet-type -s source-address -d destine-address 
 * Desc  ： the format of packet payload is like that "send timestamp"
 * Output: the result of sending packet
 *
 */
int main(int argc, char *argv[])
{
	fprintf(stdout, "Hello!\n");	

	return 0;
}
