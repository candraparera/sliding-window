#ifndef BUFFER_H
#define BUFFER_H

#include <sys/socket.h>
#include <pthread.h>

typedef struct {
    int size;
    int pos;
    char* buffer;
    pthread_mutex_t lock;
} socket_buffer;

socket_buffer* init_buffer(int size);

void free_buffer(socket_buffer* buffer);

int send_data(socket_buffer* send_buff, char* data, int len, char block);

int recv_data(socket_buffer* recv_buff, char* data, int len, char block);

void flush_send_buffer(socket_buffer* buff, int sockfd);

void create_send_recv_buffer(int sockfd, struct sockaddr* addr, pthread_t *send_buff_thread,
                             socket_buffer* send_buff, pthread_t *recv_buff_thread,
                             socket_buffer* recv_buff);

#endif