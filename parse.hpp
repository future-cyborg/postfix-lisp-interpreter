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

size_t parseList(string &str, size_t start);

size_t parseAtom(string &str, size_t start);

bool isNumber(std::string value);

std::vector<Part*> parse(string str);

std::string parseCar(string str);

std::string parseCdr(string str);

Part* listEvaluate(std::string str);

#endif