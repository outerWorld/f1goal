
#ifndef __F1G_TCP_OBJECT_H
#define __F1G_TCP_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"
#include "f1g_socket_object.h"

typedef struct _tcp_object_s {
	socket_object_t s_obj;
}tcp_object_t, *tcp_object_p;

i8_t tcp_object_init(tcp_object_p p_obj, string_t ip, u16_t port);

tcp_object_p tcp_object_create(string_t ip, u16_t port);

i8_t tcp_object_destroy(tcp_object_p p_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_TCP_OBJECT_H
