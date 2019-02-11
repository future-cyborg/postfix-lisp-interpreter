postlisp: postlisp.cpp Part.cpp parse.cpp exceptions.cpp
	g++ -std=c++11 -Wall -o $@ $^