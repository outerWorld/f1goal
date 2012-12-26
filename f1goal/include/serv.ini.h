[sys]
# multi-thread(0), multi-process(1)
work_mode = 0
# 
worker_num = 2
# single-queue(0), multi-queue(1)
que_mode = 1
# element size of queue
que_size = 512
# tcp:Long-connect(0x10), Short-connect(0x11);udp(0x20), pcap(0x30)
serv_type = 0x10
# 
serv_win = 127.0.0.0:8080
# select(0), poll(1), epoll(2)
access_mode = 0

