import sys
import socket
import time

cli = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
cli.connect((sys.argv[1], int(sys.argv[2])))
cli.send(100*"01234567890123")
print "%s" %(cli.recv(1024))
time.sleep(5) 
cli.close()
del cli
