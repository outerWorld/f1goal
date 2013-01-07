
#ifndef __F1G_BUFFER_H
#define __F1G_BUFFER_H

#include "f1g_basic_types.h"

typedef struct _buffer_s {
	i32_t	size;
	i32_t	len;
	i8_p	buf;
}buffer_t, *buffer_p;

#endif // __F1G_BUFFER_H
