
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "f1g_basic_funcs.h"
#include "f1g_data_op.h"
#include "f1g_http.h"

typedef struct tagPsuedoHdr
{
    struct in_addr stSrcAddr;
    struct in_addr stDstAddr;
    unsigned char cPlaceHolder;
    unsigned char cProtocol;
    unsigned short length;
}stPsuedoHdr_t;

#define HOST_STR_SIZE	128
#define PATH_STR_SIZE	256
#define IP_STR_SIZE		16
#define COOKIE_STR_SIZE	1024
#define UA_STR_SIZE		256
#define TYPE_STR_SIZE	16
#define PORT_STR_SIZE	8
typedef struct _webhost_info_s {
	int type;
	char src_ip[IP_STR_SIZE];
	unsigned short src_port;
	char dst_ip[IP_STR_SIZE];
	unsigned short dst_port;
	char host[HOST_STR_SIZE];
	char path[PATH_STR_SIZE];
	char ua[UA_STR_SIZE];
	char cookie[COOKIE_STR_SIZE];
}webhost_info_t, *webhost_info_p;

typedef struct _webhost_list_s {
	int webhost_num;
	webhost_info_p p_webhosts;
}webhost_list_t, *webhost_list_p;

#define LOAD_SZ 32
#define BUF_SZ 2048
#define LOAD_OFF sizeof(struct iphdr)+sizeof(struct tcphdr)	
static const g_buf_sz = BUF_SZ;
static char g_buf[BUF_SZ] = { 0 };

static webhost_list_t sg_webhost_list = { 0 };

/*
 * file format:
 *  #LINE=12;SEP=\t;\n
 * 	type\tsrc_ip\tsrc_port\tdst_ip\tdst_port\thost\tpath\tuser-agent\tcookie\t\n
 */

static char lines_str[1024] = { 0 };
static int _read_webhost_from_file(char *file, webhost_list_p p_obj)
{
	int fd;
	char *buf, *buf_end;
	char *pb, *pe, *pfield;
	struct stat st;
	int line_num;
	char sep[16];
	int len;
	int i, type, max_col;
	data_field_t df;
	
	if ((fd=open(file, O_RDONLY, 0)) <= 0) {
		fprintf(stderr, "failed to open %s [Errno=%d]\n", file, errno);
		return -1;
	}

	if (fstat(fd, &st) < 0) {
		fprintf(stderr, "failed to get file %s status [Errno=%d]\n", file, errno);
		close(fd);
		return -1;
	}

	buf = (char*)malloc(st.st_size);
	if (NULL == buf) {
		close(fd);
		return -1;
	}

	if (read(fd, buf, st.st_size) != st.st_size) {
		free(buf);
		close(fd);
		return -1;
	}

	buf_end = buf + st.st_size-1;
	pb = buf;
	find_str(pb, buf_end, "\n", &pe);
	if (NULL == pe) {
		free(buf);
		close(fd);
		return -1;
	}
	
	len = substr(pb, pe, "LINE=", ";", lines_str, 16);
	if (len <= 0) {
		free(buf);
		return -1;
	}
	lines_str[len] = '\0';
	line_num = atoi(lines_str);
	if (line_num > 1) {
		p_obj->p_webhosts = (webhost_info_p)malloc(sizeof(webhost_info_t)*line_num);
		if (NULL == p_obj->p_webhosts) {
			fprintf(stderr, "[%s,%d] Malloc failed!\n", __FUNCTION__, __LINE__);
			free(buf);
			return -1;
		}
	}
	p_obj->webhost_num = line_num;
	
	len = substr(pb, pe, "Sep=", ";", sep, 16);
	if (len <= 0) {
		memcpy(sep, "\t", 0);
		sep[1] = '\0';
	} else {
		sep[len] = '\0';
	}

	len = substr(pb, pe, "MaxCol=", ";", lines_str, 16);
	if (len <= 0) {
		max_col = 20;
	} else {
		lines_str[len] = '\0';
		max_col = atoi(lines_str);
	}
	data_field_init(&df, max_col, 0);
	
	for (i=0; i<line_num; i++) {
		pb = pe+1;
		find_str(pb, buf_end, "\n", &pe);
		if (NULL == pe) {
			pe = buf_end;
		}
		if (pb >= pe) {
			break;
		}
		data_field_read_buf(&df, pb, pe, sep);
		//data_field_print(&df);
		p_obj->p_webhosts[i].type = data_field_get_i32(&df, 0, 10);
		data_field_copy_str(&df, 1, p_obj->p_webhosts[i].src_ip, IP_STR_SIZE);	
		p_obj->p_webhosts[i].src_port = (u16_t)data_field_get_i32(&df, 2, 10);
		data_field_copy_str(&df, 3, p_obj->p_webhosts[i].dst_ip, IP_STR_SIZE);	
		p_obj->p_webhosts[i].dst_port = (u16_t)data_field_get_i32(&df, 4, 10);
		switch (p_obj->p_webhosts[i].type) {
			case 0:
				break;
			case 1:
				data_field_copy_str(&df, 5, p_obj->p_webhosts[i].host, HOST_STR_SIZE);	
				data_field_copy_str(&df, 6, p_obj->p_webhosts[i].path, PATH_STR_SIZE);	
				data_field_copy_str(&df, 7, p_obj->p_webhosts[i].ua, UA_STR_SIZE);	
				data_field_copy_str(&df, 8, p_obj->p_webhosts[i].cookie, COOKIE_STR_SIZE);	
				break;
			default:
				break;
		}
		
		data_field_clean_items(&df);
		if (pe == buf_end) {
			break;
		}
		
	}
	
	//fprintf(stdout, "Lines=%d\n", line_num);
	p_obj->webhost_num = line_num;

	data_field_free_items(&df);
	free(buf);
	close(fd);
	return st.st_size;
}

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

static int create_ip_hdr(struct iphdr *ip_hdr, unsigned int src_ip, unsigned int dst_ip, int len)
{
	memset(ip_hdr, 0x00, sizeof(struct iphdr));
	ip_hdr->ihl = 5;
	ip_hdr->version = 0x04;
	ip_hdr->tot_len = htons(len);
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


static char req_line[128] = { 0 };
static char log[2048] = { 0 };
static int log_len = 0;
/*
 * Usage: pksender conf-file sleep_time
 */
int main(int argc, char *argv[])
{
	int i;
	unsigned int sleep_time;
	struct timeval tv;
	struct timeval tv2;
	int raw_socket;
	int type;
	unsigned short src_port, dst_port;	
	unsigned int src_ip, dst_ip;
	struct iphdr *ip_hdr;
	struct tcphdr *tcp_hdr;
	struct sockaddr_in rem_addr;
	webhost_info_p p_wbi;
	http_object_p p_ho;
	int load_size;
	int data_len;
	
	if (argc < 3) {
		fprintf(stderr, "%s,%d number of arguments is not proper!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	raw_socket = create_raw_socket();
	if (raw_socket < 0) {
		fprintf(stderr, "%s,%d create raw socket fail!\n", __FUNCTION__, __LINE__);
		return -1;
	}

	memset(&sg_webhost_list, 0x00, sizeof(webhost_list_t));
	if (_read_webhost_from_file(argv[1], &sg_webhost_list) <= 0) {
		close(raw_socket);
		return -1;
	}
	sleep_time = atoi(argv[2]);

	p_ho = http_object_create(NULL, 0);

	for (i=0; i<sg_webhost_list.webhost_num; i++) {
		sleep(sleep_time);
		p_wbi = &sg_webhost_list.p_webhosts[i];
		log_len = 0;
		log[0] = '\0';
		http_object_reset(p_ho);
		if (inet_pton(AF_INET, p_wbi->src_ip, &src_ip) <= 0) {
			fprintf(stderr, "%s,%d source ip[%s] is not proper!\n", __FUNCTION__, __LINE__, p_wbi->src_ip);
			return -1;
		}
	
		if (inet_pton(AF_INET, p_wbi->dst_ip, &dst_ip) <= 0) {
			fprintf(stderr, "%s,%d source ip[%s] is not proper!\n", __FUNCTION__, __LINE__, p_wbi->dst_ip);
			return -1;
		}
		src_port = htons(p_wbi->src_port);
		dst_port = htons(p_wbi->dst_port);
		type = p_wbi->type;
		data_len = sizeof(struct iphdr) + sizeof(struct tcphdr);

		tcp_hdr = (struct tcphdr*)(g_buf+sizeof(struct iphdr));
		if (create_tcp_hdr(tcp_hdr, src_port, dst_port) != 0) {
			fprintf(stderr, "%s,%d create tcp header fail!\n", __FUNCTION__, __LINE__);
			break;
		}
		tcp_hdr->doff = 5;
		if (type == 0) {
			gettimeofday(&tv, NULL);
			sprintf(g_buf+LOAD_OFF, "&timestamp=%ld.%ld;",tv.tv_sec, tv.tv_usec);
			load_size = LOAD_SZ;
			data_len += load_size;
		} else if (type == 1) {
			req_line[0] = '\0';
			strcat(req_line, "GET ");
			strcat(req_line, p_wbi->path);
			strcat(req_line, " HTTP/1.1");
			http_object_add_header(p_ho, HTTP_HDR_REQ_LINE, req_line, strlen(req_line));
			http_object_add_header(p_ho, HTTP_HDR_HOST, p_wbi->host, strlen(p_wbi->host));
			http_object_add_header(p_ho, HTTP_HDR_COOKIE, p_wbi->cookie, strlen(p_wbi->cookie));
			http_object_add_header(p_ho, HTTP_HDR_UA, p_wbi->ua, strlen(p_wbi->ua));
			http_object_add_header(p_ho, HTTP_HDR_END, "", 0);
			//http_object_print(p_ho);
			memcpy(g_buf+LOAD_OFF, p_ho->obj_buf, p_ho->data_len);
			load_size = p_ho->data_len;
			data_len += load_size;
		}
		tcp_hdr->check = tcp_chk_sum(6, (char*)tcp_hdr, sizeof(struct tcphdr) + load_size, src_ip, dst_ip);

		ip_hdr = (struct iphdr*)g_buf;
		if (create_ip_hdr(ip_hdr, src_ip, dst_ip, data_len) != 0) {
			fprintf(stderr, "%s,%d create ip header fail!\n", __FUNCTION__, __LINE__);
			break;
		}
	
		rem_addr.sin_family = AF_INET;
		rem_addr.sin_port = dst_port;	
		rem_addr.sin_addr.s_addr = dst_ip;
		gettimeofday(&tv, NULL);
		if (sendto(raw_socket, g_buf, data_len, 0, (struct sockaddr*)&rem_addr, sizeof(struct sockaddr)) == -1) {
			fprintf(stderr, "%s,%d send packet fail[%s]!\n", __FUNCTION__, __LINE__, strerror(errno));
			break;
		}
		gettimeofday(&tv2, NULL);
		if (type == 0) {
			sprintf(log, "%s|%u|%s|%u|%ld.%ld|", p_wbi->src_ip, p_wbi->src_port, p_wbi->dst_ip, p_wbi->dst_port, tv.tv_sec, tv.tv_usec);
			sprintf(log+strlen(log), "%ld.%ld|",tv2.tv_sec, tv2.tv_usec); 
			fprintf(stdout, "%s\n", log);
		} else if (type == 1) {
			sprintf(log, "%s%s|%s|%u|%s|%u|%ld.%ld|", p_wbi->host, p_wbi->path, p_wbi->src_ip, p_wbi->src_port, p_wbi->dst_ip, p_wbi->dst_port, tv.tv_sec, tv.tv_usec);
			sprintf(log+strlen(log), "%ld.%ld|",tv2.tv_sec, tv2.tv_usec); 
			fprintf(stdout, "%s\n", log);
		}
	}

	if (sg_webhost_list.p_webhosts != NULL) {
		free(sg_webhost_list.p_webhosts);
		memset(&sg_webhost_list, 0x00, sizeof(webhost_list_t));
	}

	close(raw_socket);
	raw_socket = 0;
	//fprintf(stdout, "%s,%d send packet successfully!\n", __FUNCTION__, __LINE__);
	http_object_destroy(p_ho);
	p_ho = NULL;
	return 0;
}
