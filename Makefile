all: sender receiver

sender: client.c segment.c util.c buffer.c
	gcc client.c segment.c util.c buffer.c -o sendfile -I. -lpthread -std=gnu99

receiver: server.c segment.c util.c buffer.c
	gcc server.c segment.c util.c buffer.c -o recvfile -I. -lpthread -std=gnu99