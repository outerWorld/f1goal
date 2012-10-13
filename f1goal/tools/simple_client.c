
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <errno.h>

#include "f1g_basic_funcs.h"

#define RECV_BUF_SZ	256
#define SND_BUF_SZ	256
#define DISP_BUF_SZ	1024
static char recv_buf[RECV_BUF_SZ];
static char snd_buf[SND_BUF_SZ];
static char disp_buf[DISP_BUF_SZ];

/*
 * Usage: simple_client server_ip server_port packet_num sleep_time
 */

int main(int argc, char *argv[])
{
	int i;
	int sleep_time;
	int pack_num;
	int fd;
	int snd_len;
	int recv_len;
	int len;
	struct sockaddr_in serv_addr;
	struct sockaddr_in rem_addr;
	socklen_t addr_len;
	struct timeval tv;
	struct timeval tv2;
	char tbuf[128];

	if (argc < 5) {
		fprintf(stderr, "argc=%d is not proper!\n", argc);
		return 1;
	}
	
	memset(&serv_addr, 0x00, sizeof(struct sockaddr));
	if (1 != inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)) {
		fprintf(stderr, "inet_pton call fail [Errno=%d]\n", errno);
		return 1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "create socket fail!\n");
		return 1;
	}

	pack_num = atoi(argv[3]);
	sleep_time = atoi(argv[4]);

	for (i=0; i<pack_num; i++) {
		memset(&rem_addr, 0x00, sizeof(struct sockaddr));

		gettimeofday(&tv, NULL);
		snd_len = 0;
		snd_len = sprintf(snd_buf, "&seq=%d;&client_timeb=%ld.%ld;", i, tv.tv_sec, tv.tv_usec);
		if (sendto(fd, snd_buf, snd_len, 0, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr)) <= 0) {
			fprintf(stderr, "send data[%d bytes] fail [Errno=%d]\n", snd_len, errno);
			break;
		}

		gettimeofday(&tv, NULL);
		addr_len = sizeof(struct sockaddr);
		recv_len = recvfrom(fd, recv_buf, RECV_BUF_SZ, 0, (struct sockaddr*)&rem_addr, &addr_len);
		if (recv_len <= 0) {
			fprintf(stderr, "recv data[%d bytes] fail [Errno=%d]\n", recv_len, errno);
			break;
		}
		gettimeofday(&tv2, NULL);
		
		disp_buf[0] = '\0';
		sprintf(disp_buf+strlen(disp_buf), "%s|%s|", argv[1], argv[2]);
		len = substr(recv_buf, recv_buf+recv_len, "&seq=", ";", tbuf, 128);
		if (len <= 0) {
			continue;
		}
		sprintf(disp_buf+strlen(disp_buf), "%.*s|", len, tbuf);
		len = substr(recv_buf, recv_buf+recv_len, "&client_timeb=", ";", tbuf, 128);
		if (len <= 0) {
			continue;
		}
		sprintf(disp_buf+strlen(disp_buf), "%.*s|", len, tbuf);
		len = substr(recv_buf, recv_buf+recv_len, "&server_time=", ";", tbuf, 128);
		if (len <= 0) {
			continue;
		}
		sprintf(disp_buf+strlen(disp_buf), "%.*s|", len, tbuf);
		sprintf(disp_buf+strlen(disp_buf), "%ld.%ld|%ld.%ld|", tv.tv_sec, tv.tv_usec, tv2.tv_sec, tv2.tv_usec);
		fprintf(stdout, "%s\n", disp_buf);	
		usleep(sleep_time);
	}

	close(fd);
	return 0;
}

