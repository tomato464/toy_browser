#include"tokenize.h"
#include"parse.h"

void Generate()
{

}

void Render(char *html)
{
	Tokenize(html);

	ConstructTree();

	Generate();
}
