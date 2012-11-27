/*
 * Desc  : It provides a common used interface for reading configuration files of 
 *		   various format, and supports Multi-threads scene and Multi-processes scene.
 *
 * Author: huang chunping
 * Date  : 2012-11-27
 */

#ifndef __F1G_COMM_CONF_H
#define __F1G_COMM_CONF_H

#include "f1g_basic_types.h"

// supported configuration file types.
// 1 format of CFT_INI:
//	[section]
//		item1 = data1
//		item2 = data2
// 2 format of CFT_LINE:
//  line1: "item1" "seperator" "item2" "seperator" "item3"
//  line2: "item1" "seperator" "item2" "seperator" "item3"
// 	so, section is replaced by line number. 
enum {
	CFT_INI = 0x00,
	CFT_LINE,
};

typedef struct _comm_conf_s {
}comm_conf_t, *comm_conf_p;

i32_t comm_conf_read(string_t conf_file);

#endif // __F1G_COMM_CONF_H
