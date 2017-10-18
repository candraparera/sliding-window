#ifndef UTIL_H
#define UTIL_H

void hex(char* s, int n);
 
int to_int(char* str);

void die(char *s);

char checksum_chr(char x);

char checksum_str(char* x, int len);

void shl_buffer(char* buffer, int len, int num);

void shl_bufferl(int* buffer, int len, int num);

void shl_bufferq(long long* buffer, int len, int num);

#endif