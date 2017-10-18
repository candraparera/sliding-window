#include <stdio.h>

#include "segment.h"

int segment_to_raw(segment seg, char* buffer) {
    buffer[0] = seg.soh;
    char* x = (char*) &seg.seq;
    buffer[1] = *x;
    buffer[2] = *(x+1);
    buffer[3] = *(x+2);
    buffer[4] = *(x+3);
    buffer[5] = seg.stx;
    buffer[6] = seg.data;
    buffer[7] = seg.etx;
    buffer[8] = seg.checksum;
    return 9;
}

int ack_segment_to_raw(ack_segment seg, char* buffer) {
    buffer[0] = seg.ack;
    char* x = (char*) &seg.next_seq;
    buffer[1] = *x;
    buffer[2] = *(x+1);
    buffer[3] = *(x+2);
    buffer[4] = *(x+3);
    buffer[5] = seg.window_size;
    buffer[6] = seg.checksum;
    return 7;
}

void to_segment(char* raw, segment* seg) {
    seg->soh = *raw;
    seg->seq = ((int) *(raw+1) & 0xff) | ((int) *(raw+2) & 0xff)<<8 | ((int) *(raw+3) & 0xff)<<16 | ((int) *(raw+4) & 0xff)<<24;
    seg->stx = *(raw + 5);
    seg->data = *(raw + 6);
    seg->etx = *(raw + 7);
    seg->checksum = *(raw + 8);
}

void to_ack_segment(char* raw, ack_segment* seg) {
    seg->ack = *raw;
    seg->next_seq = ((int) *(raw+1) & 0xff) | ((int) *(raw+2) & 0xff)<<8 | ((int) *(raw+3) & 0xff)<<16 | ((int) *(raw+4) & 0xff)<<24;
    seg->window_size = *(raw + 5);
    seg->checksum = *(raw + 6);
}

void print_segment(segment seg) {
    printf("  SOH      : 0x%02x\n", seg.soh);
    printf("  segnum   : 0x%02x (%d in decimal)\n", seg.seq, seg.seq);
    printf("  STX      : 0x%02x\n", seg.stx);
    printf("  data     : 0x%02x", seg.data & 0xff);
    if (seg.data >= 0x20 && seg.data <= 0x7e)
        printf(" (%c in character)", seg.data);
    printf("\n");
    printf("  ETX      : 0x%02x\n", seg.etx);
    printf("  checksum : 0x%02x\n", seg.checksum & 0xff);
}

void print_ack_segment(ack_segment seg) {
    printf("  SOH      : %s\n", seg.ack == '\06' ? "ACK" : "NAK");
    printf("  next seq : 0x%02x (%d in decimal)\n", seg.next_seq, seg.next_seq);
    printf("  Adv.wind : 0x%02x (%d in decimal)\n", seg.window_size, seg.window_size);
    printf("  checksum : 0x%02x\n", seg.checksum & 0xff);
}