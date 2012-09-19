
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
typedef struct tagPsuedoHdr
{
    struct in_addr stSrcAddr;
    struct in_addr stDstAddr;
    unsigned char cPlaceHolder;
    unsigned char cProtocol;
    unsigned short length;
}stPsuedoHdr_t;


#define LOAD_SZ 16
#define BUF_SZ sizeof(struct iphdr)+sizeof(struct tcphdr) + LOAD_SZ
#define LOAD_OFF sizeof(struct iphdr)+sizeof(struct tcphdr)	
static const g_buf_sz = BUF_SZ;
static char g_buf[BUF_SZ] = { 0 };

static unsigned short chk_sum(unsigned short *addr,unsigned short len)
{
	register int sum = 0;
	unsigned short answer = 0;
	register unsigned short *w = addr;
	register int nleft = len;

	if( NULL == addr )
	{
		return -1;
	}
	/*
	 * Ëã·¨ÊÇÊ¹ÓÃÒ»¸ö32Î»Êý£¬ÀÛ¼Ó´ýÐ£ÑéµØÖ·Á¬ÐøµÄ16Î»£¬½«½øÎ»ÕÛ»ØµÍ16Î»
	 */
	while( nleft > 1 )
	{
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if( nleft == 1 )
	{
		*(unsigned char *)(&answer) = *(unsigned char *)w ;
		sum += answer;
	}

	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
	sum += (sum >> 16);                     /* add carry */
	answer = ~sum;                          /* truncate to 16 bits */
	return(answer);
}

static unsigned short tcp_chk_sum(unsigned proto, char *packet, unsigned short length, unsigned int src_ip, unsigned int dst_ip)
{
	struct tagPsuedoHdr stPsuedoHdr;
	struct in_addr stSrc_Addr, stDst_Addr;
	unsigned short answer;
	int sum;

	stSrc_Addr.s_addr = src_ip;
	stDst_Addr.s_addr = dst_ip;

	stPsuedoHdr.cProtocol = proto;
	stPsuedoHdr.length = htons(length);
	stPsuedoHdr.cPlaceHolder = 0;

	stPsuedoHdr.stSrcAddr = stSrc_Addr;
	stPsuedoHdr.stDstAddr = stDst_Addr;
	sum = chk_sum((unsigned short *)&stPsuedoHdr, sizeof(stPsuedoHdr));
	sum += chk_sum((unsigned short*)packet, length);

	sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
	sum += (sum >> 16);                     /* add carry */
	answer = ~sum;                          /* truncate to 16 bits */

	return answer;
}
static int create_raw_socket()
{
	unsigned short on = 1;
	int sock;
    if ((sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        return -1;
    }
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on)) < 0) {
        close(sock);
        return -1;
    }
    
	return sock;
}

static int create_ip_hdr(struct iphdr *ip_hdr, unsigned int src_ip, unsigned int dst_ip)
{
	memset(ip_hdr, 0x00, sizeof(struct iphdr));
	ip_hdr->ihl = 5;
	ip_hdr->version = 0x04;
	ip_hdr->tot_len = htons(g_buf_sz);
	ip_hdr->id = htons(getpid());
	ip_hdr->frag_off = 0;
	ip_hdr->ttl = 255;
	ip_hdr->protocol = 6;
	ip_hdr->saddr = src_ip;
	ip_hdr->daddr = dst_ip;
	ip_hdr->check = (unsigned short)chk_sum((unsigned short*)ip_hdr, sizeof(struct iphdr));

	return 0;
}

static int create_tcp_hdr(struct tcphdr *tcp_hdr, unsigned short src_port, unsigned short dst_port)
{
	memset(tcp_hdr, 0x00, sizeof(struct tcphdr));
	tcp_hdr->source = src_port;
	tcp_hdr->dest = dst_port;

	tcp_hdr->seq = 0;
	tcp_hdr->ack_seq = 0;

	tcp_hdr->res1 = 0;
	tcp_hdr->doff = 5;

	tcp_hdr->window = htons(1024);
	tcp_hdr->fin = 1;
	tcp_hdr->ack = 1;
	tcp_hdr->psh = 1;

	return 0;
}


/*
 * Usage: pksender src_ip src_port dst_ip dst_port
 */
int main(int argc, char *argv[])
{
	struct timeval tv;
	int raw_socket;
	unsigned short src_port, dst_port;	
	unsigned int src_ip, dst_ip;
	struct iphdr *ip_hdr;
	struct tcphdr *tcp_hdr;
	struct sockaddr_in rem_addr;
	
	if (argc < 5) {
		fprintf(stderr, "%s,%d number of arguments is not proper!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	if (inet_pton(AF_INET, argv[1], &src_ip) <= 0) {
		fprintf(stderr, "%s,%d source ip[%s] is not proper!\n", __FUNCTION__, __LINE__, argv[1]);
		return -1;
	}
	
	if (inet_pton(AF_INET, argv[3], &dst_ip) <= 0) {
		fprintf(stderr, "%s,%d source ip[%s] is not proper!\n", __FUNCTION__, __LINE__, argv[3]);
		return -1;
	}
	src_port = htons(atoi(argv[2]));	
	dst_port = htons(atoi(argv[4]));

	raw_socket = create_raw_socket();
	if (raw_socket < 0) {
		fprintf(stderr, "%s,%d create raw socket fail!\n", __FUNCTION__, __LINE__);
		return -1;
	}
	
	ip_hdr = (struct iphdr*)g_buf;
	if (create_ip_hdr(ip_hdr, src_ip, dst_ip) != 0) {
		fprintf(stderr, "%s,%d create ip header fail!\n", __FUNCTION__, __LINE__);
		close(raw_socket);
		raw_socket = 0;
		return -1;
	}

	tcp_hdr = (struct tcphdr*)(g_buf+sizeof(struct iphdr));
	if (create_tcp_hdr(tcp_hdr, src_port, dst_port) != 0) {
		fprintf(stderr, "%s,%d create tcp header fail!\n", __FUNCTION__, __LINE__);
		close(raw_socket);
		raw_socket = 0;
		return -1;
	}
	tcp_hdr->doff = 5;
	gettimeofday(&tv, NULL);
	sprintf(g_buf+LOAD_OFF, "%ld.%ld",tv.tv_sec, tv.tv_usec);
	tcp_hdr->check = tcp_chk_sum(6, (char*)tcp_hdr, sizeof(struct tcphdr) + LOAD_SZ, src_ip, dst_ip);
	
	rem_addr.sin_family = AF_INET;
	rem_addr.sin_port = dst_port;	
	rem_addr.sin_addr.s_addr = dst_ip;

	if (sendto(raw_socket, g_buf, g_buf_sz, 0, (struct sockaddr*)&rem_addr, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr, "%s,%d send packet fail[%s]!\n", __FUNCTION__, __LINE__, strerror(errno));
		close(raw_socket);
		raw_socket = 0;
		return -1;
	}

	close(raw_socket);
	raw_socket = 0;

	fprintf(stdout, "%s,%d send packet successfully!\n", __FUNCTION__, __LINE__);
	return 0;
}
