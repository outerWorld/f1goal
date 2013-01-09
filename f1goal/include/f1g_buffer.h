/*
 * Author: huang chunping
 * Date  : 2013-01-09
 */

#ifndef __F1G_BUFFER_H
#define __F1G_BUFFER_H

#include "f1g_basic_types.h"

typedef struct _buffer_s {
	i32_t	flag; // self allocated(0x00) which can be free by calling buffer_destroy, reference others(0x01)
	i32_t	size;
	i32_t	len;
	i8_p	buf;
}buffer_t, *buffer_p;

// allocate by self.
i32_t buffer_init(buffer_p p_buf, i32_t size);

i32_t buffer_enlarge(buffer_p p_buf, i32_t new_size);

// refer to extenal buffer
i32_t buffer_set(buffer_p p_buf, i8_p buf, i32_t size);
// 
i32_t buffer_destroy(buffer_p p_buf);

#endif // __F1G_BUFFER_H
