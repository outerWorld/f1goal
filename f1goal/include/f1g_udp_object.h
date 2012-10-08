
#ifndef __F1G_UDP_OBJECT_H
#define __F1G_UDP_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"
#include "f1g_socket_object.h"

typedef struct _udp_object_s {
	socket_object_t s_obj;
}udp_object_t, *udp_object_p;

i8_t udp_object_init(udp_object_p p_obj, string_t ip, u16_t port);

udp_object_p udp_object_create(string_t ip, u16_t port);

i8_t udp_object_destroy(udp_object_p p_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_UDP_OBJECT_H
