postlisp: postlisp.cpp Part.cpp parse.cpp
	g++ -std=c++11 -Wall -o postlisp postlisp.cpp Part.cpp parse.cpp