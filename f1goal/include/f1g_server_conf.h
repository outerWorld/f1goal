
#ifndef __F1G_SERVER_CONF_H
#define __F1G_SERVER_CONF_H

#define SERV_WIN_DESC_SIZE	256

typedef struct _server_conf_s {
	// working mode: multi-thread(0) or multi-process(1)
	int 	work_mode;
	// number of worker thread or process
	int 	worker_num;
	// queue mode:single-queue(0), or multi-queue(1)
	int 	que_mode;
	// queue size: element size of queue
	int		que_size;
	// service type:tcp long connection (0x10) or tcp short connection(0x11), or udp(0x20), or pcap(0x30)
	int 	serv_type;
	// service window: ip:port for tcp or udp, netdevice and filter condition for pcap.
	char 	serv_win[SERV_WIN_DESC_SIZE];
	// access mode:select(0), poll(1), epoll(2)
	int		access_mode;
}server_conf_t, *server_conf_p;

#endif // __F1G_SERVER_CONF_H
