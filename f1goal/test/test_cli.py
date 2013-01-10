import sys
import socket
import time
data=100*"0123456789"
data += 24*"a"
data += "b"
#data = data * 2
cli = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
cli.connect((sys.argv[1], int(sys.argv[2])))
for i in range(2):
	send_bytes = cli.send(data)
	print "sent %d bytes [%s]" %(send_bytes, data)
	print "recv %s" %(cli.recv(1024))

Continue=""
input(Continue)

cli.close()
del cli
