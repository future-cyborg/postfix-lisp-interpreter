#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "exceptions.hpp"
#include "Part.hpp"

#ifndef PARSE_H
#define PARSE_H

using std::string;
using std::stringstream;


/*
 *	isNumber() returns true if the string represents an integer value
 */
bool isNumber(std::string value);

/*
 * 	parseList() returns the position of the closing parenthesis that
 * 		matches the first starting parenthesis.
 *
 *	The position at str of start must be '('
 *
 *	Will throw Exception if no matching parenthesis is found,
 */
size_t parseList(string &str, size_t start);

/*
 *	parseAtom() returns the position of the first parenthesis or whitespace found.
 *
 */
size_t parseAtom(string &str, size_t start);

/*
 *  parse() parses a string and returns vector of Parts
 */
std::vector<Part*> parse(string str);

/*
 *	parseCar() returns the first element of a list (represented as a string)
 */
std::string parseCar(string str);

/*
 *	parseCdr() returns all but the first element of a list (represented as a string)
 */
std::string parseCdr(string str);

#endif