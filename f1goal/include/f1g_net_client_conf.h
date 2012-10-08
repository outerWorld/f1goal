
#ifndef __F1G_NET_CLIENT_CONF_H
#define __F1G_NET_CLIENT_CONF_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"

typedef struct _net_client_conf_s {
	i8_t 	ip[IP_STR_LEN];
	u16_t   port;
	u32_t	r_timeout; // ms
	u32_t	s_timeout; // ms
	i32_t	s_buf_size; // size of send buffer
	i32_t	r_buf_size; // size of receive buffer
}net_client_conf_t, *net_client_conf_p;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_NET_CLIENT_CONF_H
