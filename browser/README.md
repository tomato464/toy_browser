In this repository, I make basic browser.
send http-request, get http response, parse html, show markdown

how it works :

get URL from cmd line.
(http://host/path)
parse URL, and get scheme, host, path.
make HTTP request, and send it.
get HTTP response.
parse HTML sentences.
make DOM tree.
{
create TOKEN array from html.
struct TOKEN{ type; tag_name; data, next;}
create TREE from token.
}
show HTML markdown.
