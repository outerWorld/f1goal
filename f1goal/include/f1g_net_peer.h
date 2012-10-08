
#ifndef __F1G_NET_PEER_H
#define __F1G_NET_PEER_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"

typedef struct _net_peer_object_s {
	struct _net_peer_object_s * next;
	string_t 					ip[IP_STR_LEN];
	u16_t						port;
}net_peer_object_t, *net_peer_object_p;

typedef struct _net_peers_s {
	u16_t             peer_num;
	net_peer_object_p p_head;	
}net_peer_head_t, *net_peers_p;


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_NET_PEER_H
