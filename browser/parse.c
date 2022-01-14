#include"parse.h"
#include"tokenize.h"
#include<stddef.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

Node *root = NULL;
Node *current = NULL;

Node *stack[100];
int count = 0;
static int PushNode(Node *node)
{
	if(count >= 100){
		return 0;
	}
	if(count < 0){
		return 0;
	}

	stack[count] = node;
	count++;
	return 1;
}

static Node *PopNode()
{
	if(count < 0){
		return NULL;
	}
	if(count > 100){
		return NULL;
	}

	count--;

	return stack[count];
}

static void InsertNode(Node *node)
{
	node->parent = current;
	if(!current->first_child){
		//this is first child
		current->first_child = node;
	}else{
		node->previous_sibling = current->last_child;
		current->last_child->next_sibling = node;
		
	}
	current->last_child = node;
}

static Node *CreateNode(NodeType node_type)
{
	Node *node;
	node = malloc(sizeof(Node));
	memset(node, 0, sizeof(Node));
	node->node_type = node_type;

	return node;
}

static Node *CreateNodeElement(ElementType element_type, Token *token)
{
	Node *node = CreateNode(ELEMENT);
	node->element_type = element_type;
	node->tag_name = token->tag_name;
	return node;
}

static Node *CreateNodeChrFromToken(Token **token_pp)
{
	Token *token;
	token = *token_pp;
	Node *node = CreateNode(TEXT);
	node->data = (char *) malloc(100);
	strcpy(node->data, &token->data);
	token = token->next;
	while(token->type == CHR){
		if(!node){
			break;
		}
		strcat(node->data, &token->data);
		token = token->next;
	}
	*token_pp = token;
	return node;
}

void ConstructTree()
{
	NodeMode mode = INITIAL;
	root = CreateNode(DOCUMENT);
	current = root;
	Node *node = NULL;
	Token *token = first_token;
	Token **token_pp;
	token_pp = &token;

	while(token){
		switch(mode){
		case INITIAL :{
			mode = BEFORE_HTML;
			continue;
		}
	
		case BEFORE_HTML :{
			if(token->type == StartTag && !strcmp(token->tag_name, "html")){
				node = CreateNodeElement(HTML, token);
				InsertNode(node);
				PushNode(current);
				current = node;
				token = token->next;
				mode = BEFORE_HEAD;
				continue;
			}
		}
	
		case BEFORE_HEAD :{
			if(token->type == StartTag && !strcmp(token->tag_name, "head")){
				node = CreateNodeElement(HEAD, token);
				InsertNode(node);
				PushNode(current);
				current = node;
				token = token->next;
				mode = IN_HEAD;
				continue;
			}

				
			{// other
			node = CreateNode(ELEMENT);
			node->element_type = HEAD;
			InsertNode(node);
			PushNode(current);
			current = node;
			mode = IN_HEAD;
			continue;
			}
		}
	
		case IN_HEAD :{
			if(token->type == EndTag && !strcmp(token->tag_name, "head")){
				current = PopNode();
				token = token->next;
				mode = AFTER_HEAD;
				continue;
			}
		
			{//other
			current = PopNode();
			mode = AFTER_HEAD;
			continue;
			}
		}
	
		case AFTER_HEAD :{
			if(token->type == StartTag && !strcmp(token->tag_name, "body")){
				node = CreateNodeElement(BODY, token);
				InsertNode(node);
				PushNode(current);
				current = node;
				token = token->next;
				mode = IN_BODY;
				continue;
			}
		}
	
		case IN_BODY :{
			if(token->type == CHR){
				node = CreateNodeChrFromToken(token_pp);
				InsertNode(node);
				continue;
			}
	
			if(token->type == StartTag && !strcmp(token->tag_name, "h1")){
				node = CreateNodeElement(HEADING, token);
				InsertNode(node);
				PushNode(node);
				current = node;
				token = token->next;
				continue;
			}
		
			if(token->type == EndTag && !strcmp(token->tag_name, "body")){
				current = PopNode();
				token = token->next;
				mode = AFTER_BODY;
				continue;
			}
	
			if(token->type == EndTag){
				current = PopNode();
				token = token->next;
				continue;
			}
		
		}
	
		case AFTER_BODY :{
			mode = AFTER_AFTER_BODY;
			continue;
		}

		case AFTER_AFTER_BODY :{
			if(token->type == EndTag && !strcmp(token->tag_name, "html")){
				current = PopNode();
				token = token->next;
				break;
			}
		}
	
		default :
	
			break;
		}
	}
}

static void PrintElement(char *name, int depth)
{
	printf("------%s------", name);
	printf("------%d------\n", depth);
}

static void Print(Node *node, int depth)
{
	if(node->node_type == TEXT){
		printf("%s\n", node->data);
		return;
	}

	if(node->node_type == DOCUMENT){
		printf("------DOC--------");
		printf("-------%d--------\n", depth);
		return;
	}

	if(node->element_type == HEAD){
		printf("----head------");
		printf("-----%d-------\n", depth);
		return;
	}

	PrintElement(node->tag_name, depth);

	return;
}

static void PrintRecursive(Node *node, int depth)
{
	Print(node, depth);
	if(depth == 100){
		return;
	}
	for(Node *cur = node->first_child; cur; cur = cur->next_sibling){
		PrintRecursive(cur, depth + 1);
	}

}

void TestParse(char *html)
{
	Tokenize(html);
	ConstructTree();
	PrintRecursive(root, 0);	
}
