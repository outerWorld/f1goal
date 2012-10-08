
#ifndef __F1G_NET_OBJECT_H
#define __F1G_NET_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"
#include "f1g_socket_object.h"
#include "f1g_udp_object.h"
#include "f1g_tcp_object.h"

typedef enum {
	TCP_SOCKET = 0x00,
	UDP_SOCKET,
	SOCKET_TYPE_NUM
}SO_TYPE_E;

typedef struct _net_object_s {
	SO_TYPE_E 		so_type;
	socket_object_p p_objs[SOCKET_TYPE_NUM];
}net_object_t, net_object_p;

i8_t net_object_init(net_object_p p_no, SO_TYPE_E so_type, string_t ip, u16_t port);

net_object_p net_object_create(SO_TYPE_E so_type, string_t ip, u16_t port);

i8_t net_object_destroy(net_object_p p_obj);

i8_t net_object_send(net_object_p p_obj, string_t data, i32_t data_len, string_ip, u16_t port);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // __F1G_NET_OBJECT_H
