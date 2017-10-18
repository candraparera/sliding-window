all: sender receiver

sender: src/client.c src/segment.c src/util.c src/buffer.c
	gcc src/client.c src/segment.c src/util.c src/buffer.c -o sendfile -I. -lpthread -std=gnu99

receiver: src/server.c src/segment.c src/util.c src/buffer.c
	gcc src/server.c src/segment.c src/util.c src/buffer.c -o recvfile -I. -lpthread -std=gnu99