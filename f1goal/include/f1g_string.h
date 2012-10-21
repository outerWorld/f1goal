
#ifndef __F1G_STRING_H
#define __F1G_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "f1g_basic_types.h"

// variable
typedef struct _v_string_s {
	i32_t size;
	i32_t len;
	i8_p  data;	
}v_string_t, *v_string_p;

// const
typedef struct _c_string_s {
	i32_t len;
	i8_p  data;	
}c_string_t, *c_string_p;


i8_t c_string_set(c_string_p p_str, i8_p data, i32_t len);

#ifdef __cplusplus
}
#endif 

#endif // __F1G_STRING_H
