#!/bin/bash

Assert(){

	input="$1"
	expect="$2"

	actual=$(./browser.bin --url "$input" --url_test)

	if [ "$actual" = "$expect" ];
	then
		echo "input $input => $actual"
	else
		echo "input $input : expect $expect but got $actual"
		exit 1
	fi
}

echo "test start ...."

make clean
make

Assert "http://host/path" "Scheme : http
Host : host
Port : 80
Path : /path"


Assert "http://123.123.123/page1.html" "Scheme : http
Host : 123.123.123
Port : 80
Path : /page1.html"

Assert "http://123.123.123" "Scheme : http
Host : 123.123.123
Port : 80
Path : /"

Assert "http://123.123.123:8888/path" "Scheme : http
Host : 123.123.123
Port : 8888
Path : /path"

echo "--------------------"
echo "---pass all test----"
echo "--------------------"
