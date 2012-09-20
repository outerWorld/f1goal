
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
i32_t main(i32_t argc, i8_p argv[])
{
	i32_t i;
	i8_t data[128];
	
	for (i=0; i<128; i++) {
		data[i] = i;
	}
	hex_print(stdout, data, 125);

	return 0;
}
