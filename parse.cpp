#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "exceptions.hpp"
#include "Part.hpp"

#ifndef PARSE_H
#define PARSE_H

using std::string;
using std::stringstream;


size_t parseList(string &str, size_t start) {
	//std::cout << "start: " << start << std::endl;
	size_t prev = start, pos;
	int stack = 0;
	while( (pos = str.find_first_of(parens, prev)) != std::string::npos) {
		if(str.at(pos) == '(') {
			++stack;
		} else {
			if(stack) {
				--stack;
			} else {
				return pos;
			}
		}
		prev = pos + 1;
	}

	throw UnmatchingParenthesis();
}

size_t parseAtom(string &str, size_t start) {
	return str.find_first_of(whiteParen, start);
}

bool isNumber(std::string value) {
	for(char ch : value) {
 		if(!isdigit(ch)) {
 			return false;
 		}
 	}
 	return true;
}

std::vector<Part*> parse(string str) {
	std::vector<Part*> parts;

	size_t prev = 0, pos, end;
	while( (pos = str.find_first_not_of(whitespaces, prev)) != std::string::npos) {
		// std::cout << "prev: " << prev << '\n';
		// std::cout << "pos: " << str.find_first_not_of(whitespaces, prev) << "|" << std::endl;
		switch(str.at(pos)) {
			case '(': {
				end = parseList(str, pos + 1);
				// make a list
				parts.push_back(new List(str.substr(pos + 1, end - pos - 1)));

				// std::cout << "list: " << str.substr(pos + 1, end - pos - 1) << "|" << std::endl;
				break;
			}
			case ')': {
				throw UnmatchingParenthesis();
			}
			default: {
				end = parseAtom(str, pos + 1);
				// Make atom
				std::string atom = str.substr(pos, end - pos);
				if(isNumber(atom)) {
					parts.push_back(new Number(atom));
				} else {
					parts.push_back(new Atom(atom));
				}
				// std::cout << "atom: " << str.substr(pos, end - pos) << "|" << std::endl;;
				break;
			}
		}
		if(end == std::string::npos) break;
		prev = end + 1;
	}
	std::reverse(parts.begin(), parts.end());
	return parts;
}

std::string parseCar(string str) {
	size_t pos, end;
	pos = str.find_first_not_of(whitespaces, 0);
	if(pos == std::string::npos) throw Exception("parseCar error");

	if(str.at(pos) == '(') {
		end = parseList(str, pos + 1);
	} else {
		end = parseAtom(str, pos + 1);
	}
	return str.substr(pos, end - pos);
}

std::string parseCdr(string str) {
	size_t pos, end;
	pos = str.find_first_not_of(whitespaces, 0);
	if(pos == std::string::npos) throw Exception("parseCar error");

	if(str.at(pos) == '(') {
		end = parseList(str, pos + 1);
	} else {
		end = parseAtom(str, pos + 1);
	}
	pos = str.find_first_not_of(whitespaces, end + 1);
	return str.substr(pos);
}


Part* callFunction(Part* command, std::vector<Part*> args);

// Must free memory
Part* listEvaluate(std::string str);



#endif