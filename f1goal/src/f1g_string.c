
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_string.h"


i8_t c_string_set(c_string_p p_str, i8_p data, i32_t len)
{
	p_str->data = data;
	p_str->len = len;

	return F1G_OK;
}
