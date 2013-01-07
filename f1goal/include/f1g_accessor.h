
#ifndef __F1G_ACCESSOR_H
#define __F1G_ACCESSOR_H

#include "f1g_basic_types.h"

typedef struct _accessor_s {
	i32_t	fd;
}accessor_t, *accessor_p;

i32_t accessor_init(accessor_p p_acc);

accessor_p accessor_create();

i32_t accessor_detect(accessor_p p_acc);

i32_t accessor_get_data(accessor_p p_acc);

i32_t accessor_clear();

i32_t accessor_destroy();

#endif // __F1G_ACCESSOR_H
