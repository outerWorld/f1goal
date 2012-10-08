
#ifndef __F1G_UDP_H
#define __F1G_UDP_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"

typedef struct _udp_object_s {
	i32_t fd;
	i8_t  ip[IP_STR_LEN];	
	u16_t port;
}udp_object_t, *udp_object_p;


udp_object_p udp_object_create(string_t ip, u16_t port);

i8_t udp_object_init(string_t ip, u16_t port, udp_object_p p_obj);


i8_t udp_object_destroy(udp_object_p p_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_UDP_H
