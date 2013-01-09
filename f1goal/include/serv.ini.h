[sys]
# multi-thread(0), multi-process(1)
work_mode = 0
# 
worker_num = 2
# single-queue(0), multi-queue(1)
que_mode = 1
# element size of queue
que_size = 512
que_cap = 1024
# tcp:Long-connect(0x10), Short-connect(0x11);udp(0x20), pcap(0x30)
serv_type = 0x10
# for epoll
serv_win = ip:127.0.0.0;port:8080;socket_size:100;read_timeout:10;write_timeout:10;read_buf:1024;write_buf:1024
# for pcap
# serv_win = linkname:eth0;filter:tcp dst port 80
# epoll(0), poll(1), select(1)  
access_mode = 0
