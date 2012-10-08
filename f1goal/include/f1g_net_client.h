
#ifndef __F1G_NET_CLIENT_H
#define __F1G_NET_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"
#include "f1g_socket_object.h"
#include "f1g_net_object.h"
#include "f1g_net_peer.h"

typedef struct _net_client_s {
	net_object_p 	net_obj;
	net_peers_t 	peers;
}net_client_t, *net_client_p;

net_client_p net_client_create(SO_TYPE_E type);

i8_t net_client_destroy(net_client_p p_obj);

i8_t net_client_load_config(string_t conf_file);

i8_t net_client_add_peer(net_client_p p_obj, string_t ip, u16_t port);

i8_t net_client_send(net_client_p p_obj, string_t data, i32_t data_len);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_NET_CLIENT_H
