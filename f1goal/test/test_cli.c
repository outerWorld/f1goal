
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
static char write_buf[BUF_SIZE];
static char read_buf[BUF_SIZE];

int main(int argc, char *argv[])
{
	int len = 0;
	int cli_fd = 0;
	char * serv_ip;
	unsigned short serv_port;
	struct sockaddr_in serv_addr;

	if (argc < 3) {
		serv_port = 8888;
		serv_ip = "127.0.0.1";
	} else {
		serv_ip = argv[1];
		serv_port = atoi(argv[2]);
	}
	
	if (argc > 3) {
		len = snprintf(write_buf, BUF_SIZE, "%s", argv[3]);
	} else {
		len = snprintf(write_buf, BUF_SIZE, "%s is running", argv[0]);
	}

	if (len >= BUF_SIZE) {
		fprintf(stderr, "format write_buf fail, too long\n");
		return 1;
	}

	cli_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (cli_fd <= 0) {
		return 1;
	}

	memset(&serv_addr, 0x00, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	if (1 != inet_pton(AF_INET, serv_ip, (void*)&serv_addr.sin_addr)) {
		return 1;
	}
	serv_addr.sin_port = htons(serv_port);
	if (0 != connect(cli_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
		return 1;
	}

	if (len != send(cli_fd, write_buf, len, 0)) {
		fprintf(stderr, "send fail [%d,%s]\n", errno, strerror(errno));	
	}

	len = recv(cli_fd, read_buf, BUF_SIZE, 0);
	if (0 == len) {
		fprintf(stdout, "disconnected from server!\n");
	} else if (-1 == len) {
		// check errno first!!!
		//
		fprintf(stderr, "recv fail [%d,%s]\n", errno, strerror(errno));
		return 1;
	}

	fprintf(stdout, "recv %d bytes [%s]\n", len, read_buf);

	close(cli_fd);

	return 0;
}
