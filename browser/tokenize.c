#include"tokenize.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

Token *first_token = NULL, *current_token = NULL;

typedef enum{
	DATA,
	TagOpen,
	TagName,
	EndTagOpen,
}Status;

static Token *CreateToken(TokenType type)
{
	Token *new_token;
	new_token = (Token *) malloc(sizeof(Token));

	new_token->type = type;
	new_token->next = NULL;

	return new_token;
}

static void AppendToken(Token *token)
{
	if(!current_token){
		//first time
		first_token = token;
		current_token = token;
	}else{
		current_token->next = token;
		current_token = token;
	}
}

static Token *CreateChrToken(char data)
{
	Token *token = CreateToken(CHR);
	token->data = data;
	return token;
}

void Tokenize(char *html)
{
	Status mode = DATA;
	while(*html){
			if(*html == 0x000d){
				*html = 0x000a;
			}

			switch(mode){
			case DATA :{
				if(*html == '<'){
					mode = TagOpen;
					html++;
					break;
				}
				Token *token = CreateChrToken(*html);
				AppendToken(token);
				html++;
				break;
			}
			case TagOpen :{
				if(*html == '/'){
					mode = EndTagOpen;
					html++;
					break;
				}
				Token *token = CreateToken(StartTag);
				token->tag_name = malloc(10);
				AppendToken(token);
				mode = TagName;
				break;
			}
			case TagName :{
				if(*html == '>'){
					mode = DATA;
					html++;
					break;
				}
				strncat(current_token->tag_name, html, 1);
				html++;
				break;
			}
			case EndTagOpen :{
				Token *token = CreateToken(EndTag);
				token->tag_name = malloc(10);
				AppendToken(token);
				mode = TagName;
				break;
			}
			default :
				//error
				break;
			}
	}
}

void PrintToken()
{
	Token *token = first_token;
	while(token){
		switch(token->type){
		case CHR :{
			printf("%c", token->data);
			token = token->next;
			break;
		}
		case StartTag :{
			printf("<%s>", token->tag_name);
			token = token->next;
			break;
		}
		case EndTag :{
			printf("</%s>", token->tag_name);
			token = token->next;
			break;
		}
		default :
			break;
		}
	}
}

void TestTokenize(char *html)
{
	Tokenize(html);
	PrintToken();
}
