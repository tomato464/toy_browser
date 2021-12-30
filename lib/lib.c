#include"lib.h"


uint8_t StrToByte(const char *s, const char **next)
{
	uint8_t v = 0;
	while('0' <= *s && *s <= '9'){
		v = 10*v + (*s - '0');
		s++;
	}

	if(next){
		*next = s;
	}

	return v;
}

void PrintIPaddr(in_addr_t ip_addr)
{
	uint8_t buf[4];
	*(uint32_t *) buf = ip_addr;
	for(int i = 0; i < 4; i++){
		printf("%d", buf[i]);
		if(i == 3){
			break;
		}
		printf(".");
	}
}

in_addr_t GetIPaddrFromString(const char *s)
{
// "a.b.c.d" => buf[] (uint32_t)
	uint8_t buf[4];

	for(int i = 0; ; i++){

		buf[i] = StrToByte(s, &s);

		if(i == 3){
			break;
		}

		if(*s == '.'){
			s++;
		}else{
			printf("invalid ip addr : should use '.'\n");
			exit(1);
		}

	}	

	return *(in_addr_t *)buf;
}

uint16_t StrToNum16(const char *s, const char **next)
{
	uint16_t v = 0;
	while('0' <= *s && *s <= '9'){
		v = 10*v + (*s - '0');
		s++;
	}
	if(next){
		*next = s;
	}

	return v;
}

void PrintNStr(const char *s, int n)
{
	for(int i = 0; i < n; i++){
		printf("%c", s[i]);
	}
}
