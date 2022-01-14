#ifndef _TOKENIZE_H_
#define _TOKENIZE_H_

#include<stddef.h>

typedef enum {
	CHR,
	StartTag,
	EndTag,
} TokenType;

struct Token{
	TokenType type;
	char *tag_name;
	char data;//character for string
	struct Token *next;
};

typedef struct Token Token;

extern Token *first_token, *current_token;

void Tokenize(char *html);

void PrintToken();

void TestTokenize(char *html);

#endif
