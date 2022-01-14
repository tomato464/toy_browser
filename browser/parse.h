#ifndef _PARSE_H_
#define _PARSE_H_

typedef enum{
	INITIAL,
	BEFORE_HTML,
	BEFORE_HEAD,
	IN_HEAD,
	AFTER_HEAD,
	IN_BODY,
	AFTER_BODY,
	AFTER_AFTER_BODY,
}NodeMode;

typedef enum{
	DOCUMENT,
	ELEMENT,
	TEXT,
}NodeType;

typedef enum{
	HTML,
	HEAD,
	BODY,
	HEADING,
	DIV,
	PARAGRAPH,
	UL,
	LI,
	NONE,
}ElementType;

typedef struct Node{
	NodeType node_type;
	ElementType element_type;
	char *tag_name;

	char *data;//for char

	struct Node *parent;
	struct Node *first_child;
	struct Node *last_child;
	struct Node *previous_sibling;
	struct Node *next_sibling;
}Node;

void ConstructTree(void);
void TestParse(char *html);
extern Node *root;

#endif
