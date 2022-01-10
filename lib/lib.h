#ifndef _LIB_H_
#define _LIB_H_

#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stddef.h>

typedef uint8_t bool;
#define true 1
#define false 0

typedef uint32_t in_addr_t;

uint8_t StrToByte(const char *s, const char **next);

void PrintIPaddr(in_addr_t ip_addr);

in_addr_t GetIPaddrFromString(const char *s);

uint16_t StrToNum16(const char *s, const char **next);

void PrintNStr(const char *s, const int n);
#endif
