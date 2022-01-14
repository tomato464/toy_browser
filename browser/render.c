#include"tokenize.h"
#include"parse.h"
#include<stdio.h>
#include<string.h>

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
			printf("*");
		}
		if(!strcmp(node->tag_name, "h2")){
			printf("**");
		}
		if(!strcmp(node->tag_name, "h3")){
			printf("***");
		}
		if(!strcmp(node->tag_name, "h4")){
			printf("****");
		}


		break;
	}

	case DIV :{
		break;
	}

	case PARAGRAPH :{
		break;
	}

	case UL :{
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
		printf("%s", node->data);
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
