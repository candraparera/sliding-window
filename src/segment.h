#ifndef SEGMENT_H
#define SEGMENT_H

typedef struct {
    char soh;
    int seq;
    char stx;
    char data;
    char etx;
    char checksum;
} segment;

typedef struct {
    char ack;
    int next_seq;
    char window_size;
    char checksum;
} ack_segment;

int segment_to_raw(segment seg, char* buffer);

int ack_segment_to_raw(ack_segment seg, char* buffer);

void to_segment(char* raw, segment* seg);

void to_ack_segment(char* raw, ack_segment* seg);

void print_segment(segment seg);

void print_ack_segment(ack_segment seg);

#endif