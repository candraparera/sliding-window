#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "buffer.h"

socket_buffer* init_buffer(int size) {
    socket_buffer* new_buffer = (socket_buffer*) malloc(sizeof(socket_buffer));
    new_buffer->size = size;
    new_buffer->pos = 0;
    new_buffer->buffer = (char*) malloc(size * sizeof(char));
    pthread_mutex_init(&new_buffer->lock, NULL);
    return new_buffer;
}

void free_buffer(socket_buffer* buffer) {
    free(buffer->buffer);
    pthread_mutex_destroy(&buffer->lock);
    free(buffer);
}

int send_data(socket_buffer* send_buff, char* data, int len, char block) {
    int sent = 0;
    while (len) {
        pthread_mutex_lock(&send_buff->lock);
        if (!block && send_buff->size - send_buff->pos < len) {
            pthread_mutex_unlock(&send_buff->lock);
            return 0;
        }
        while (len > 0 && send_buff->pos < send_buff->size) {
            send_buff->buffer[send_buff->pos++] = *(data++);
            len--;
            sent++;
        }
        pthread_mutex_unlock(&send_buff->lock);
        if (len)
            usleep(abs(rand() % 10000));
    }
    return sent;
}

int recv_data(socket_buffer* recv_buff, char* data, int len, char block) {
    int read = 0;
    while (len) {
        pthread_mutex_lock(&recv_buff->lock);
        if (!block && len > recv_buff->pos) {
            pthread_mutex_unlock(&recv_buff->lock);
            return 0;
        }
        while (len > 0 && recv_buff->pos > 0) {
            *(data++) = *recv_buff->buffer;
            read++;
            int i = 0;
            for (; i < recv_buff->pos - 1; i++)
                recv_buff->buffer[i] = recv_buff->buffer[i+1];
            len--;
            recv_buff->pos--;
        }
        pthread_mutex_unlock(&recv_buff->lock);
    }
    return read;
}

struct __thread_buffer {
    int sockfd;
    socket_buffer* buffer;
};

struct sockaddr __os_sender_address;
int __os_sender_address_size;

void flush_send_buffer(socket_buffer* buff, int sockfd) {
    char* data = (char*) malloc(256);
    int len = 0;
    pthread_mutex_lock(&buff->lock);
    while (buff->pos > 0 && len < 256) {
        data[len++] = *buff->buffer;
        int i = 0;
        for (; i < buff->pos - 1; i++)
            buff->buffer[i] = buff->buffer[i+1];
        buff->pos--;
    }
    pthread_mutex_unlock(&buff->lock);
    sendto(sockfd, data, len, 0, &__os_sender_address, __os_sender_address_size);
    free(data);
}

void *__send_buffer_to_os(void* param) {
    int sockfd = ((struct __thread_buffer*) param)->sockfd;
    socket_buffer* buff = ((struct __thread_buffer*) param)->buffer;
    free(param);

    char* data = (char*) malloc(256);
    memset(data, 0, 256);
    int len = 0;
    while (1) {
        flush_send_buffer(buff, sockfd);
        usleep(rand() % 10000);
    }
    free(data);
}

void *__recv_buffer_from_os(void* param) {
    int sockfd = ((struct __thread_buffer*) param)->sockfd;
    socket_buffer* buff = ((struct __thread_buffer*) param)->buffer;
    free(param);

    __os_sender_address_size = sizeof(__os_sender_address);

    char* data = (char*) malloc(256);
    memset(data, 0, 256);
    while (1) {
        int len = recvfrom(sockfd, data, 255, 0, &__os_sender_address, &__os_sender_address_size);
        int i = 0;
        while (len > 0) {
            pthread_mutex_lock(&buff->lock);
            if (buff->pos < buff->size) {
                buff->buffer[buff->pos++] = data[i++];
                len--;
            }
            pthread_mutex_unlock(&buff->lock);
        }

        usleep(rand() % 10000);
    }
    free(data);
}

void create_send_recv_buffer(int sockfd, struct sockaddr* addr, pthread_t *send_buff_thread,
                             socket_buffer* send_buff, pthread_t *recv_buff_thread,
                             socket_buffer* recv_buff) {
    pthread_t send_tid, recv_tid;

    struct __thread_buffer* param_1 = (struct __thread_buffer*) malloc(sizeof(struct __thread_buffer));
    struct __thread_buffer* param_2 = (struct __thread_buffer*) malloc(sizeof(struct __thread_buffer));

    param_1->sockfd = param_2->sockfd = sockfd;
    param_1->buffer = send_buff;
    param_2->buffer = recv_buff;

    if (addr != NULL) {
        __os_sender_address = *addr;
        __os_sender_address_size = sizeof __os_sender_address;
    }

    pthread_create(&send_tid, NULL, &__send_buffer_to_os, (void*) param_1);
    pthread_create(&recv_tid, NULL, &__recv_buffer_from_os, (void*) param_2);
    
    if (send_buff_thread != 0)
        *send_buff_thread = send_tid;

    if (recv_buff_thread != 0)
        *recv_buff_thread = recv_tid;
}