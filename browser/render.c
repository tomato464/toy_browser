#include"tokenize.h"
#include"parse.h"
#include<stdio.h>
#include<string.h>
#include"../lib/lib.h"

bool first_line = true;

static bool IsPrintable(char *txt)
{
	while(*txt){
		if(*txt != 0x09 &&
			*txt != 0x0a &&
			*txt != 0x0c &&
			*txt != 0x0d &&
			*txt != 0x20){
			return true;
		}
		txt++;
	}
	return false;
}

static void MarkdownText(Node *node)
{
	if(IsPrintable(node->data)){
		printf("%s", node->data);
		first_line = false;
	}

}

static void MarkdownElement(Node *node)
{
	switch(node->element_type){
	case HTML :{
		break;
	}

	case HEAD :{
		break;
	}

	case BODY :{
		break;
	}

	case HEADING :{
		if(!strcmp(node->tag_name, "h1")){
			printf("* ");
		}
		if(!strcmp(node->tag_name, "h2")){
			printf("** ");
		}
		if(!strcmp(node->tag_name, "h3")){
			printf("*** ");
		}
		if(!strcmp(node->tag_name, "h4")){
			printf("**** ");
		}
		first_line = false;
		break;
	}

	case DIV :{
		if(!first_line){
			printf("\n");
		}
		break;
	}

	case PARAGRAPH :{
		if(!first_line){
			printf("\n");
		}
		break;
	}

	case UL :{
		if(!first_line){
			printf("\n");
		}
		break;
	}

	case LI :{
		if(!first_line){
			printf("\n");
		}
		printf("- ");
		first_line = false;
		break;
	}

	default :{
		break;
	}

	}
}

static void Markdown(Node *node)
{
	switch(node->node_type){
	case DOCUMENT:{

		break;
	}

	case ELEMENT :{
		MarkdownElement(node);
		break;
	}

	case TEXT :{
		MarkdownText(node);
		break;
	}

	default :{
		break;
	}

	}
	return;
}

static void MarkDfs(Node *node)
{
	Markdown(node);

	for(Node *cur = node->first_child; cur; cur = cur->next_sibling){
		MarkDfs(cur);	
	}
}

static void Generate(void)
{
	Node *node = root;

	MarkDfs(node);
}

void Render(char *html)
{
	Tokenize(html);

	ConstructTree();

	Generate();
	printf("\n");
}
