/*
 * Author: huang chunping
 * Date  : 2013-01-09
 */

#ifndef __F1G_BUFFER_H
#define __F1G_BUFFER_H

#include "f1g_basic_types.h"

enum {
	BUF_FLAG_ALLO = 0x00, 	// allocated itself
	BUF_FLAG_REF, 			// reference of other memory
	BUF_FLAG_UNK, 			//unknown flag
};
typedef struct _buffer_s {
	i32_t	flag; // self allocated(0x00) which can be free by calling buffer_destroy, reference others(0x01)
	i32_t	size;
	i32_t	len;
	i8_p	buf;
}buffer_t, *buffer_p;

// allocate by self.
i32_t buffer_init(buffer_p p_buf, i32_t size);

i32_t buffer_enlarge(buffer_p p_buf, i32_t new_size);

//
i32_t buffer_append(buffer_p p_buf, i8_p p_data, i32_t data_len);

i32_t buffer_append_format(buffer_p p_buf, string_t fmt, ...);

// refer to extenal buffer
i32_t buffer_set(buffer_p p_buf, i8_p buf, i32_t size);

// clear data in buffer, also its length.
i32_t buffer_clear(buffer_p p_buf);

i8_p buffer_data(buffer_p p_buf);
// 
i32_t buffer_data_len(buffer_p p_buf);
i32_t buffer_size(buffer_p p_buf);

// 
i32_t buffer_destroy(buffer_p p_buf);

i32_t buffer_show(buffer_p p_buf);

#endif // __F1G_BUFFER_H
