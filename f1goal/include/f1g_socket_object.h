
#ifndef __F1G_SOCKET_OBJECT_H
#define __F1G_SOCKET_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"

typedef struct _socket_object_s {
	i32_t fd;
	i8_t  ip[IP_STR_LEN];
	u16_t port;
}socket_object_t, *socket_object_p;

i8_t socket_object_init(socket_object_p p_so, stringt_t ip, u16_t port);

socket_object_p socket_object_create(string_t ip, u16_t port);

i8_t socket_object_set_rbuf_size(socket_object_p p_so, u32_t r_size);

i8_t socket_object_set_sbuf_size(socket_object_p p_so, u32_t s_size);

i8_t socket_object_set_block(socket_object_p p_so, u8_t blk_flag);

i8_t socket_object_set_rtimeout(socket_object_p p_so, u32_t r_to);

i8_t socket_object_set_stimeout(socket_object_p p_so, u32_t r_to);

i8_t socket_object_destroy(socket_object_p p_so);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_SOCKET_OBJECT_H
