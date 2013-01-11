
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
// under different platform (Ubuntu,fedora and others),
//	the location header file is different too,
//	so should add a Macro to control the include
//#include <linux/eventpoll.h>
#include <sys/epoll.h>

#include "f1g_accessor.h"

i32_t sock_info_show(sock_info_p p_si)
{
	i8_t ip[24] = { 0 };
	if (NULL == inet_ntop(AF_INET, (void*)&p_si->src_ip,
	 							  ip, 24)) {
		fprintf(stderr, "inet_ntop fail!fd=%d, src_ip=%08x, src_port=%u, status=%d\n",
				p_si->fd, p_si->src_ip, p_si->src_port, p_si->sock_status);
		return F1G_ERR;
	}
	fprintf(stdout, "fd=%d, src_ip=%s, src_port=%u, status=%d\n",
			p_si->fd, ip, p_si->src_port, p_si->sock_status);

	return F1G_OK;
}

i32_t nonblk_sendto(i32_t fd, u32_t ip, u16_t port, buffer_p p_buf, i32_t *send_stat)
{
	struct sockaddr_in addr;

	memset(&addr, 0x00, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip;
	addr.sin_port = port;

	sendto(fd, buffer_data(p_buf), buffer_data_len(p_buf), 0, (struct sockaddr*)&addr, sizeof(addr));

	return F1G_OK;
}

i32_t nonblk_recvfrom(i32_t fd, buffer_p p_buf, i32_t *recv_stat)
{
	i32_t len = 0;
	i32_t recv_flag = 0; // 0 (continue), 1(complete)
	i32_t wait_times = 0;
	i32_t recv_len = 0;
	socklen_t addr_len = 0;
	struct sockaddr_in addr;

	*recv_stat = RECV_INIT;
	addr_len = sizeof(addr);
	while (!recv_flag && len < p_buf->size) {
		recv_len = recvfrom(fd, p_buf->buf + len, p_buf->size-len, 0, (struct sockaddr*)&addr, &addr_len);
		if (-1 == recv_len) {
			//fprintf(stdout, "%s errno = %d,%s\n", __FUNCTION__, errno, strerror(errno));
			switch (errno) {
				case EINTR:
				case EAGAIN:
				//case EWOULDBLOCK:
					if (++wait_times >= 3) {
						*recv_stat |= RECV_TIMEOUT;
						recv_flag = 1;
					}
					usleep(1);	// wait for data be ok
					break;
				case EBADF:
				case ECONNREFUSED:
				case EFAULT:
				case EINVAL:
				case ENOTSOCK:
				case ENOTCONN:
					*recv_stat &= RECV_ERR;
					*recv_stat &= ~RECV_OK;
					recv_flag = 1;
					break;
				default:
					*recv_stat &= RECV_ERR;
					*recv_stat &= ~RECV_OK;
					recv_flag = 1;
					break;
			}
		} else if (0 == recv_len) {
			// disconnected by peer.
			*recv_stat |= RECV_DISC;
			recv_flag = 1;
		} else {
			*recv_stat |= RECV_OK;
			len += recv_len;
			//fprintf(stdout, "%s get %d bytes\n", __FUNCTION__, recv_len);
		}
	}

	if (len >= p_buf->size) {
		*recv_stat |= RECV_MEM_LACK;
		//fprintf(stdout, "%s detect RECV_MEM_LACK\n", __FUNCTION__);
	}
	p_buf->len = len;
	p_buf->buf[len] = '\0';
	//fprintf(stdout, "%s total %d bytes\n", __FUNCTION__, len);

	return F1G_OK;
}

static i32_t set_sockfd_nonblk(i32_t fd)
{
	int opts;

	opts = fcntl(fd, F_GETFL);
	if (opts < 0) return F1G_ERR; 

    opts = opts | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, opts) < 0) return F1G_ERR;

	return F1G_OK;
}

// paras format is
//	ip:127.0.0.1;port:8080;socket_size:100;read_timeout:10;write_timeout:10;
static i32_t epoller_init(epoller_p p_ep, i32_t sock_type, string_t paras)
{
	i32_t	i = 0;
	struct sockaddr_in addr;
	struct epoll_event ev;
	sock_info_p p_add_info = NULL;

	p_ep->accessor_id = epoll_create(512);
	if (-1 == p_ep->accessor_id) {
		//fprintf(stderr, "epoll_create fail [%d,%s]\n", errno, strerror(errno));
		return F1G_ERR;
	}
	
	p_ep->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	// set noblock
	if (F1G_OK != set_sockfd_nonblk(p_ep->listen_fd)) return F1G_ERR;

	memset(&ev, 0x00, sizeof(struct epoll_event));
	ev.events = EPOLLIN | EPOLLET;
	//ev.data.fd = p_ep->listen_fd;
	p_add_info = (sock_info_p)malloc(sizeof(sock_info_t));
	if (!p_add_info) return F1G_ERR;
	memset(p_add_info, 0x00, sizeof(sock_info_t));
	p_add_info->fd = p_ep->listen_fd;
	p_add_info->src_ip = addr.sin_addr.s_addr;
	p_add_info->src_port = addr.sin_port;
	ev.data.ptr = p_add_info;
	if (0 != epoll_ctl(p_ep->accessor_id, EPOLL_CTL_ADD, p_ep->listen_fd, &ev)) {
		//fprintf(stderr, "epoll_ctl EPOLL_CTL_ADD %d fail [%d,%s]\n", p_ep->listen_fd, errno, strerror(errno));
		return F1G_ERR;
	}

	memset(&addr, 0x00, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8888);

	if (0 != bind(p_ep->listen_fd, (struct sockaddr*)&addr, sizeof(addr))) {
		return F1G_ERR;
	}

	if (0 != listen(p_ep->listen_fd, 64)) {
		//fprintf(stderr, "%d listen failed [%d,%s]\n", p_ep->listen_fd, errno, strerror(errno));
		return F1G_ERR;
	}

	p_ep->event_size = 512;
	p_ep->events = (struct epoll_event*)malloc(sizeof(struct epoll_event)*p_ep->event_size);
	if (!p_ep->events) return F1G_ERR;
	memset(p_ep->events, 0x00, sizeof(struct epoll_event)*p_ep->event_size);
	p_ep->event_num = 0;

	return F1G_OK;
}

i32_t accessor_init(accessor_p p_acc, i32_t link_type, i32_t sock_type, string_t paras)
{
	buffer_init(&p_acc->rdbuf, 1024);
	buffer_init(&p_acc->wrbuf, 1024);

	switch (link_type) {
		case LINKER_EPOLL:
			p_acc->linkers[LINKER_EPOLL] = malloc(sizeof(epoller_t));
			p_acc->link_mode = LINKER_EPOLL;
			if (!p_acc->linkers[LINKER_EPOLL]) return F1G_ERR;
			if (F1G_OK != epoller_init((epoller_p)p_acc->linkers[LINKER_EPOLL], sock_type, paras)) return F1G_ERR;
			break;
		
		case LINKER_POLL:
			break;

		case LINKER_SELECT:
			break;

		case LINKER_PCAP:
			break;

		case LINKER_USER_DEF:
			break;

		default:
			return F1G_ERR;
	}

	return F1G_OK;
}

accessor_p accessor_create(i32_t type, i32_t sock_type, string_t paras)
{
	accessor_p p_acc = NULL;

	p_acc = (accessor_p)malloc(sizeof(accessor_t));
	if (!p_acc) return NULL;
	memset(p_acc, 0x00, sizeof(accessor_t));

	if (F1G_OK != accessor_init(p_acc, type, sock_type, paras)) {
		free(p_acc);
		return NULL;
	}

	return p_acc;
}

i32_t accessor_detect(accessor_p p_acc)
{
	i32_t n = 0;
	//struct epoll_event evs[20];
	epoller_p p_ep = NULL;

	switch (p_acc->link_mode) {
		case LINKER_EPOLL:
			p_ep = (epoller_p)p_acc->linkers[LINKER_EPOLL];
			n = epoll_wait(p_ep->accessor_id, p_ep->events, p_ep->event_size, 500);
			//n = epoll_wait(p_ep->accessor_id, evs, 20, 500);
			p_ep->event_num = n;
			p_ep->next_event = 0;
			break;
		default:
			break;
	}

	if (n < 0) {
		//fprintf(stderr, "epoll_wait fail [%d,%s]\n", errno, strerror(errno));
		// check whether the error can be ignored, if so, set n be 0
		if (EINTR == errno) {
			n = 0;
		}
	} else if (n > 0) {
		//fprintf(stdout, "epoll_wait [%d] events \n", n);
	}

	return n;
}

i32_t accessor_check_status(accessor_p p_acc, sock_info_p p_si)
{
	i32_t n = 0;
	i32_t ev_fd = 0;
	i32_t cli_fd = 0;
	i32_t recv_stat = 0;
	epoller_p p_ep = NULL;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	struct epoll_event * ev;
	struct epoll_event new_ev;
	struct sockaddr_in cli_addr;
	sock_info_p p_add_info = NULL;

	switch (p_acc->link_mode) {
		case LINKER_EPOLL:
			p_ep = (epoller_p)p_acc->linkers[LINKER_EPOLL];
			if (p_ep->next_event < p_ep->event_num) {
				ev = &p_ep->events[p_ep->next_event];
				// process event
				p_add_info = ev->data.ptr;
				if (p_add_info->fd == p_ep->listen_fd) {
					cli_fd = accept(p_ep->listen_fd, (struct sockaddr*)&cli_addr, &addr_len);
					if (-1 == cli_fd) {
						//fprintf(stderr, "listener %d accept fail [%d,%s]\n", p_ep->listen_fd, errno, strerror(errno));
					} else {
						//fprintf(stdout, "listener %d accept %d successfully\n", p_ep->listen_fd, cli_fd);
						set_sockfd_nonblk(cli_fd);
						memset(&new_ev, 0x00, sizeof(struct epoll_event));
						new_ev.events = EPOLLIN | EPOLLET | EPOLLERR | EPOLLHUP;
						//new_ev.events = EPOLLIN | EPOLLERR | EPOLLHUP;
						p_add_info = (sock_info_p)malloc(sizeof(sock_info_t));
						if (!p_add_info) return 0;
						p_add_info->sock_status = SST_INIT;
						p_add_info->fd = cli_fd;
						p_add_info->src_ip = cli_addr.sin_addr.s_addr;
						p_add_info->src_port = cli_addr.sin_port;
						memcpy(p_si, p_add_info, sizeof(sock_info_t));
						//fprintf(stdout, "accept from %08x:%u, add_len=%d\n", cli_addr.sin_addr.s_addr, cli_addr.sin_port, addr_len);
						// i think the add_ev.data.ptr = (fd,cli_addr) is more beautiful.
						//new_ev.data.fd = cli_fd;
						new_ev.data.ptr = p_add_info;
						if (0 != epoll_ctl(p_ep->accessor_id, EPOLL_CTL_ADD, cli_fd, &new_ev)) {
							//fprintf(stderr, "epoll_ctl EPOLL_CTL_ADD client fd %d fail\n", cli_fd);
							return 0;
						}
					}
				} else {
					p_add_info = ev->data.ptr;
					memcpy(p_si, p_add_info, sizeof(sock_info_t));
					p_si->sock_status = SST_UNKNOWN; // because it won't process all the events occured
					ev_fd = p_add_info->fd;
					//fprintf(stdout, "events[%08x] for socket[%d]\n", ev->events, ev_fd);
					if (ev->events & EPOLLHUP) {
						//fprintf(stdout, "events EPOLLHUP for [%d]\n", ev_fd);
						p_si->sock_status = SST_DISC;
						new_ev.events = 0;
						if (p_add_info) free(p_add_info);
						p_add_info = NULL;
						if (0 != epoll_ctl(p_ep->accessor_id, EPOLL_CTL_DEL, ev_fd, &new_ev)) {
							//fprintf(stderr, "epoll_ctl EPOLL_CTL_DEL %d fail\n", cli_fd);
							return 0;
						}
						close(ev_fd);
					}
					if (ev->events & EPOLLERR) {
						//fprintf(stdout, " event EPOLLERR for [%d]\n", cli_fd);
						p_si->sock_status = SST_ERR;
						new_ev.events = 0;
						if (p_add_info) free(p_add_info);
						p_add_info = NULL;
						if (0 != epoll_ctl(p_ep->accessor_id, EPOLL_CTL_DEL, ev_fd, &new_ev)) {
							//fprintf(stderr, "epoll_ctl EPOLL_CTL_DEL %d fail [%d,%s]\n", cli_fd, errno, strerror(errno));
							return 0;
						}
						close(ev_fd);
					}
					if (ev->events & EPOLLIN) {
						//fprintf(stdout, "events EPOLLIN for [%d]\n", ev_fd);
						p_si->sock_status = SST_DATA_IN;
					}
					if (ev->events & EPOLLOUT) {
						//fprintf(stdout, "events EPOLLOUT for [%d]\n", ev_fd);
						p_si->sock_status = SST_DATA_OUT;
					}
					// other events
					{
						//fprintf(stdout, "events %08x for [%d]", ev->events, ev_fd);
						// not processed.
					}
					//fprintf(stdout, "2 events[%08x] [EPOLLIN=%08x]for socket[%d]\n", ev->events, EPOLLIN, ev_fd);
				}
				// set next event to be processed
				p_ep->next_event++;
			} else {
				p_ep->next_event = 0;
			}
			n = p_ep->next_event;
			//if (n >= p_ep->event_num) { n = 0; }
			break;

		default:
			break;
	}

	return n;
}

i32_t accessor_clear(accessor_p p_acc)
{
	epoller_p p_ep = NULL;
	
	switch (p_acc->link_mode) {
		case LINKER_EPOLL:
			p_ep = (epoller_p)p_acc->linkers[LINKER_EPOLL];
			p_ep->event_num = 0;
			p_ep->next_event = 0;
			break;
		default:
			break;
	}
	
	return F1G_OK;
}

i32_t accessor_destroy(accessor_p p_acc)
{
	buffer_destroy(&p_acc->rdbuf);
	buffer_destroy(&p_acc->wrbuf);

	return F1G_OK;
}

