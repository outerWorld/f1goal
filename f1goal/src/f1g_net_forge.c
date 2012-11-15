
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/in.h>

#include "f1g_net_forge.h"

u16_t chk_sum(u16_p addr, u16_t len)
{
	u16_t answer = 0;
	register i32_t sum = 0;
	register u16_p w = addr;
	register i32_t nleft = len;

	if (NULL == addr) {
		return -1;
	}

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(u8_p)(&answer) = *(u8_p)w ;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return (answer);
}

i32_t chk_sum2(u16_p addr, u16_t len)
{
	u16_t answer = 0;
	register i32_t sum = 0;
	register u16_p w = addr;
	register i32_t nleft = len;

	if (NULL == addr) {
		return -1;
	}

	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		*(u8_p)(&answer) = *(u8_p)w ;
		sum += answer;
	}

	return (sum);
}


u16_t tcp_chk_sum(u8_t proto, i8_p packet, u16_t length, u32_t src_ip, u32_t dst_ip)
{
	i32_t sum;
	u16_t answer;
	psuedo_hdr_t psd_hdr;

	psd_hdr.protocol = proto;
	psd_hdr.length = htons(length);
	psd_hdr.place_holder = 0;

	psd_hdr.src_addr.s_addr = src_ip;
	psd_hdr.dst_addr.s_addr = dst_ip;

	sum = chk_sum((u16_p)&psd_hdr, sizeof(psuedo_hdr_t));
	sum += chk_sum((u16_p)packet, length);

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return answer;
}

i32_t net_forge_tcp_packet(value_p p_buf, net_forge_para_p p_para)
{
	i32_t ret;
	u16_t pack_len = 0;
	struct iphdr * 	p_iphdr;
	struct tcphdr * p_tcphdr;

	p_iphdr = (struct iphdr*)(p_buf->val);
	p_tcphdr = (struct tcphdr*)(p_buf->val + sizeof(struct iphdr));	

	
	pack_len = sizeof(struct iphdr) + sizeof(struct tcphdr) + p_para->load_len;
	p_buf->val_len = pack_len;

	// Create ip header.
	memset(p_iphdr, 0x00, sizeof(struct iphdr));
	p_iphdr->ihl = 5;
	p_iphdr->version = 0x04;
	p_iphdr->tot_len = htons(pack_len);
	p_iphdr->id = htons(getpid());
	p_iphdr->frag_off = 0;
	p_iphdr->ttl = 255;
	p_iphdr->protocol = IPPROTO_TCP;
	p_iphdr->saddr = p_para->src_ip;
	p_iphdr->daddr = p_para->dst_ip;
	p_iphdr->check = (u16_t)chk_sum((u16_p)p_iphdr, sizeof(struct iphdr));

	// Create tcp header.
	memset(p_tcphdr, 0x00, sizeof(struct tcphdr));
	p_tcphdr->source = p_para->src_port;
	p_tcphdr->dest = p_para->dst_port;
	p_tcphdr->seq = p_para->seq;
	p_tcphdr->ack_seq = p_para->ack;
	p_tcphdr->res1 = 0;
	p_tcphdr->doff = 5;
	p_tcphdr->window = htons(1024);
	p_tcphdr->fin = (0 != (p_para->tcp_flag&TCP_FLAG_FIN));
	p_tcphdr->ack = (0 != (p_para->tcp_flag&TCP_FLAG_ACK));
	p_tcphdr->psh = (0 != (p_para->tcp_flag&TCP_FLAG_PSH));
	p_tcphdr->check = tcp_chk_sum(IPPROTO_TCP, (i8_p)p_tcphdr, sizeof(struct tcphdr)+p_para->load_len, p_para->src_ip, p_para->dst_ip);
	
	return F1G_OK;
}
