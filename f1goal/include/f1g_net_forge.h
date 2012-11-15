
#ifndef __F1G_NET_FORGE_H
#define __F1G_NET_FORGE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "f1g_basic_types.h"

#define TCP_FLAG_FIN 0x01
#define TCP_FLAG_SYN 0x02
#define TCP_FLAG_RST 0x04
#define TCP_FLAG_PSH 0x08
#define TCP_FLAG_ACK 0x10
#define TCP_FLAG_URG 0x20

typedef struct _psuedo_hdr_s {
    struct in_addr 	src_addr;
    struct in_addr 	dst_addr;
    u8_t			place_holder;
    u8_t	 		protocol;
    u32_t	 		length;
}psuedo_hdr_t, *psuedo_hdr_p;

typedef struct _net_forge_para_s {
	u32_t	seq;
	u32_t	ack;
	u32_t	src_ip;
	u32_t	dst_ip;
	u16_t	dst_port;
	u16_t	src_port;
	u8_t	tcp_flag;
	u16_t	load_len;
	i8_p	load;
}net_forge_para_t, *net_forge_para_p;

u16_t chk_sum(u16_p addr, u16_t len);
i32_t chk_sum2(u16_p addr, u16_t len);

u16_t tcp_chk_sum(u8_t proto, i8_p packet, u16_t length, u32_t src_ip, u32_t dst_ip);

i32_t net_forge_tcp_packet(value_p p_buf, net_forge_para_p p_para);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __F1G_NET_FORGE_H
