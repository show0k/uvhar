###
import socket
import sys

server_port = 5556;
MAX_TO_READ = 1024;

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
try:
    s.bind(('', server_port))
except socket.error, err:
    print "Couldn't be a udp server on port %d : %s" % (
            server_port, err)
    raise SystemExit

while True:
    datagram = s.recv(MAX_TO_READ)
    if not datagram:
        break
    # do somethingA
    print datagram
s.close()

# or 

"""
import SocketServer

class handler(SocketServer.DatagramRequestHandler):
    def handle(self):
        # do something (with self.request[0])

s = SocketServer.UDPServer(('',10000), handler)
s.serve_forever()
"""
