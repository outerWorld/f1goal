import sys
import socket
import time
data=80*"0123456789"
data += 24*"a"
data += "b"
#data = data * 2
cli = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
cli.connect((sys.argv[1], int(sys.argv[2])))
for i in range(1):
	send_bytes = cli.send(data)
	print "sent %d bytes [%s]" %(send_bytes, data)
	recvd = cli.recv(1024)
	print "recv %d bytes %s" %(len(recvd), recvd)

a = raw_input("please enter any key for quit:")

cli.close()
del cli
