/*
 * Desc  : It provides a common used interface for reading configuration files of 
 *		   various format, and supports Multi-threads scene and Multi-processes scene.
 *
 * Author: huang chunping
 * Date  : 2012-11-27
 */

#ifndef __F1G_SMART_CONF_H
#define __F1G_SMART_CONF_H

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

typedef struct _smart_conf_s {
}smart_conf_t, *_smart_conf_p;

i32_t smart_conf_read(smart_conf_p p_conf, string_t conf_file, i32_t conf_type);
i32_t smart_conf_destroy(smart_conf_p p_conf);

#endif // __F1G_SMART_CONF_H
