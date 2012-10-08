
#ifndef __F1G_UDP_CLIENT_H
#define __F1G_UDP_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"
#include "f1g_udp.h"

typedef struct _peer_object_s {
	string_t ip;
	u16_t    port;
}peer_object_t, *peer_object_p;

typedef struct _udp_peers_s {
	peer_object_t peer_objs[];	
}udp_peers_t, *udp_peers_p;

typedef struct _udp_client_s {
	udp_object_t base;
	udp_peers_t peers;
}udp_client_t, *udp_client_p;

udp_client_p udp_client_create(string_t ip, u16_t port);

i8_t udp_client_destroy(udp_client_p p_obj);

i8_t udp_client_add_peer(udp_client_p p_obj, string_t peer_ip, u16_t peer_port);

i8_t udp_client_send(udp_client_p p_obj, string_t data, i32_t data_len);

i8_t udp_client_recv(udp_client_p p_obj, string_t buffer, i32_t buffer_size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_UDP_CLIENT_H
