#ifndef _LIB_H_
#define _LIB_H_

#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef uint32_t in_addr_t;

uint8_t StrToByte(const char *s, const char **next);

void PrintIPaddr(in_addr_t ip_addr);

in_addr_t GetIPaddrFromString(const char *s);

unsigned short StrToNum16(const char *s, const char **next);

void PrintNStr(const char *s, const int n);
#endif
