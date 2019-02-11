postlisp: postlisp.cpp Part.cpp parse.cpp exceptions.cpp functions.cpp
	g++ -std=c++11 -Wall -o $@ $^