
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

#define RECV_BUF_SZ	128
#define SND_BUF_SZ	256
static char recv_buf[RECV_BUF_SZ];
static char snd_buf[SND_BUF_SZ];

/*
 * Usage: simple_server ip port
 */
int main(int argc, char *argv[])
{
	int fd;
	int port_reuse = 0;
	struct sockaddr_in addr;
	struct sockaddr_in remote_addr;
	socklen_t addr_len;
	ssize_t recv_len;
	ssize_t snd_len;
	struct timeval tv;
	int ret;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "create socket fail!\n");
		return 1;
	}

	memset(&addr, 0x00, sizeof(struct sockaddr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) != 0) {
		close(fd);
		return 0;
	}
	
#if 0
	port_reuse = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &port_reuse, sizeof(int)) == -1) {
		close(fd);
		return 0;
	}
#endif

	while (1) {
		addr_len = sizeof(struct sockaddr);
		recv_len = recvfrom(fd, recv_buf, RECV_BUF_SZ, 0, (struct sockaddr*)&remote_addr, &addr_len);
		if (-1 == recv_len) {
			fprintf(stderr, "recvfrom call failed [Errno=%d]\n", errno);
			break;
		}
		fprintf(stdout, "read data[%.*s]\n", recv_len, recv_buf);
		snd_len = recv_len;
		gettimeofday(&tv, NULL);
		memcpy(snd_buf, recv_buf, recv_len);
		snd_len += snprintf(snd_buf + recv_len, SND_BUF_SZ-recv_len, "&server_time=%ld.%ld;", tv.tv_sec, tv.tv_usec);
		ret = sendto(fd, snd_buf, snd_len, 0, (struct sockaddr*)&remote_addr, addr_len);
		if (ret < 0) {
			fprintf(stderr, "send data fail [Errno=%d]\n", errno);
		} else if (snd_len != ret) {
			fprintf(stderr, "there is [%d] bytes not sent\n", snd_len - ret);
		}
	}

	close(fd);
	return 0;
}
