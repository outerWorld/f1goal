
#ifndef __F1G_UDP_SERVER_H
#define __F1G_UDP_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"
#include "f1g_udp.h"

typedef struct _udp_server_s {
	udp_object_t base;	
}udp_server_t, *udp_server_p;


udp_server_p udp_server_create(string_t ip, u16_t port);

i8_t udp_server_destroy(udp_server_p p_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_UDP_SERVER_H
