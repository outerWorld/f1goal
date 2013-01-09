import sys
import socket

cli = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
cli.connect(('127.0.0.1', 8888))
cli.send("i'm client")
cli.recv(1024)
cli.close()
del cli
