
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ether.h>

#include "pcap.h"
#include "f1g_eth_scrapy.h"

#define FILTER_STR_SZ	128
#define LOG_BUF_SZ	256
typedef struct packet_info_s
{
        int sec;
        int usec;
        unsigned int cap_len;
        unsigned int len;
}packet_info_t, *packet_info_p;

static int g_running_flag = 1;
static char g_log_buf[LOG_BUF_SZ];

static void handler(u_char *user, const struct pcap_pkthdr *pkt_hdr, const u_char *pkt_data)
{
	char src_ip[24];
	char dst_ip[24];
	char * load = NULL;
	int trans_off = 0;
	int load_sz = 0;
	struct timeval tv;
	struct iphdr * ip_hdr = NULL;
	struct tcphdr * tcp_hdr = NULL;
	struct udphdr * udp_hdr = NULL;
	struct ether_header * eth_hdr = NULL;
	unsigned short src_port, dst_port;

	//gettimeofday(&tv, NULL);
	packet_info_p pkt_info = (packet_info_p)pkt_hdr;
	eth_hdr = (struct ether_header*)pkt_data;
	if (ntohs(eth_hdr->ether_type) != ETHERTYPE_IP) {
		fprintf(stderr, "[%ld.%ld] packet is not ip packet![type=%04x]\n", tv.tv_sec, tv.tv_usec, ntohs(eth_hdr->ether_type));
		return;
	}
	
	g_log_buf[0] = '\0';
	ip_hdr = (struct iphdr*)(pkt_data + 14);
	if (inet_ntop(AF_INET, &ip_hdr->saddr, &src_ip) < 0) {
		return;	
	}
	
	if (inet_ntop(AF_INET, &ip_hdr->daddr, &dst_ip) < 0) {
		return;
	}
	
	trans_off = 14 + (ip_hdr->ihl*4);
	if (ip_hdr->protocol == IPPROTO_TCP) {
		strcat(g_log_buf, "tcp|");
		tcp_hdr = (struct tcphdr*)(pkt_data + trans_off);
		load_sz = pkt_hdr->len -  trans_off - (tcp_hdr->doff*4);
		load = (char *)(pkt_data + trans_off + (tcp_hdr->doff*4));
		src_port = ntohs(tcp_hdr->source);
		dst_port = ntohs(tcp_hdr->dest);
	} else if (ip_hdr->protocol == IPPROTO_UDP) {
		strcat(g_log_buf, "udp|");
		udp_hdr = (struct udphdr *)(pkt_data + trans_off);
		load_sz = pkt_hdr->len - trans_off - sizeof(struct udphdr);
		src_port = ntohs(udp_hdr->source);
		dst_port = ntohs(udp_hdr->dest);
		load = (char *)(pkt_data + trans_off + sizeof(struct udphdr));
	} else {
		return;
	}
	
	if (load_sz == 16) {
		sprintf(g_log_buf + strlen(g_log_buf), "%s|%ld.%ld|%s|%u|%s|%u", load, pkt_hdr->ts.tv_sec, pkt_hdr->ts.tv_usec, src_ip, src_port, dst_ip, dst_port);
		//sprintf(g_log_buf + strlen(g_log_buf), "%s|%ld.%ld|%s|%u|%s|%u", load, tv.tv_sec, tv.tv_usec, src_ip, src_port, dst_ip, dst_port);
		fprintf(stdout, "%s\n", g_log_buf);
	} else {
		fprintf(stderr,"load size is [%d]\n", load_sz);
	}
}


/*
 * Usage: pkreceiver -i eth0 [tcp src port 80]
 */
int main(int argc, char *argv[])
{
	int ret;
	int i;
	char filter[FILTER_STR_SZ] = { 0 };
	
	if (argc < 3) {
		fprintf(stderr, "%s,%d arguments are not proper!\n", __FUNCTION__, __LINE__);
		return 1;
	}

	for (i=3; i<argc; i++) {
    	strcat(filter, argv[i]);
        strcat(filter, " ");
    }
	
	if (eth_scrapy_init(argv[2], filter) < 0) {
		fprintf(stderr, "%s,%d eth_scrapy_init fail!\n", __FUNCTION__, __LINE__);
		return 1;
	}	

	if (eth_scrapy_reg(handler) < 0) {
		fprintf(stderr, "%s,%d eth_scrapy_reg fail!\n", __FUNCTION__, __LINE__);
		eth_scrapy_destroy();
		return 1;
	}	

	if (eth_scrapy_start() < 0) {
		fprintf(stderr, "%s,%d eth_scrapy_start fail!\n", __FUNCTION__, __LINE__);
		eth_scrapy_destroy();
		return 1;
	}
	
	while (g_running_flag) {
		
		sleep(5000000);
	}

	eth_scrapy_control(ETH_SCRAPY_STOP);
	eth_scrapy_destroy();

	return 0;
}
