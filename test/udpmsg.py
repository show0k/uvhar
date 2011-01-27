#!/usr/bin/python
# udpmsg - send a message to the udpquotd server

import socket
import sys

MAXLEN = 1024
PORTNO = 5151
TIMEOUT = 5

server_host = sys.argv[1]
msg = " ".join(sys.argv[2:])

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.settimeout(TIMEOUT)
sock.connect((server_host, PORTNO))
sock.send(msg)
try:
    msg = sock.recv(MAXLEN)
    ipaddr, port = sock.getpeername()
    hishost = socket.gethostbyaddr(ipaddr)
    print "Server %s responded ``%s''" % ( hishost[0], msg)
except:
    print "recv from %s failed (timeout or no server running)." % (
            server_host )
sock.close()
